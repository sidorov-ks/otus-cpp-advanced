#include "async.h"
#include "block_processor.h"

#include "io/producers.h"
#include "io/consumers.h"

#include <queue>
#include <mutex>
#include <condition_variable>

LogQueue log_queue;
FileQueue file_queue;
auto callback = make_producer(&log_queue, &file_queue);

void async::startup() {
  std::thread(log_thread_fn, &log_queue).detach();
  std::thread(file_thread_fn, &file_queue, true).detach();
  std::thread(file_thread_fn, &file_queue, false).detach();
}

async::handle_t async::connect(std::size_t bulk) {
  BlockProcessor *processor = new BlockProcessor{bulk, callback};
  return processor;
}

void receive(BlockProcessor *processor, const std::string &line) {
  if (line.empty()) return;
  else if (line == async::BLOCK_OPEN) { // HACK move out of async
    processor->open();
  } else if (line == async::BLOCK_CLOSE) {
    processor->close();
  } else {
    processor->feed(line);
  }
}

void async::receive(handle_t handle, const char *data, std::size_t size) {
  auto *processor = reinterpret_cast<BlockProcessor *>(handle);
  std::string line;
  for (std::size_t ix = 0; ix <= size; ++ix) {
    if (ix == size || data[ix] == '\n') {
      receive(processor, line);
      line.clear();
      processor->send_block();
    } else {
      line += data[ix];
    }
  }
}

void async::disconnect(handle_t handle) {
  auto *processor = reinterpret_cast<BlockProcessor *>(handle);
  processor->halt();
  processor->send_block();
  delete processor;
}