#include "async.h"
#include "block_processor.h"

#include "io/producers.h"
#include "io/consumers.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

std::shared_ptr<ProducerQueues> make_queues() {
  static ProducerQueues queues{LogQueue{}, std::atomic<unsigned char>{false}, FileQueue{}};
  static std::shared_ptr<ProducerQueues> ptr{&queues};
  return ptr;
}

BlockProcessor::CallbackType make_producer() {
  auto queues = make_queues();
  static auto producer = make_producer(queues);
  return producer;
}

void process_connection_event(bool is_connect) {
  static std::atomic<std::size_t> conn_counter{0};
  static std::vector<std::thread> threads;
  if (is_connect) {
    auto conn_count = conn_counter.fetch_add(1);
    if (conn_count == 0) {
      auto queues = make_queues();
      threads.emplace_back(log_thread_fn, queues);
      threads.emplace_back(file_thread_fn, queues, true);
      threads.emplace_back(file_thread_fn, queues, false);
    }
  } else {
    auto conn_count = conn_counter.fetch_sub(1);
    if (conn_count == 1) {
      for (auto &thread: threads) {
        if (thread.joinable())
          thread.join();
      }
    }
  }
}

async::handle_t async::connect(std::size_t bulk) {
  process_connection_event(true);
  async::handle_t handle = new BlockProcessor{bulk, make_producer()};
  return handle;
}

void receive(BlockProcessor *processor, const std::string &line) {
  if (line.empty()) return;
  else if (line == async::BLOCK_OPEN) {
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
  process_connection_event(false);
}