#include "consumers.h"

#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

std::string make_line(const std::list<std::string> &lines) {
  std::stringstream ss;
  ss << "bulk: ";
  for (auto it = lines.cbegin(); it != lines.cend(); it++) {
    ss << *it;
    auto it_next = it;
    if (++it_next != lines.end()) ss << ", ";
  }
  return ss.str();
}

void log_thread_fn(std::shared_ptr<ProducerQueues> queues) {
  while (true) {
    std::unique_lock lock(queues->log_queue.mutex);
    queues->log_queue.empty_cv.wait(lock, [&queues]() { return !queues->log_queue.queue.empty(); });
    const Block &block = queues->log_queue.queue.front();
    std::cout << make_line(block.lines) << std::endl;
    queues->log_queue.queue.pop();
  }
}

void file_thread_fn(std::shared_ptr<ProducerQueues> queue, bool parity) {
  auto filename_suffix = parity ? "odd" : "even";
  while (true) {
    std::unique_lock lock(queue->file_queue.mutex);
    queue->file_queue.empty_cv.wait(lock,
                                    [&queue, parity]() {
                                      return !queue->file_queue.queue.empty() &&
                                             queue->file_queue.queue.front().second == parity;
                                    });
    const auto &[block, queue_parity] = queue->file_queue.queue.front();
    assert(parity == queue_parity);
    std::ofstream fout("bulk" + std::to_string(block.block_id.time_since_epoch().count()) + "_" + filename_suffix);
    fout << make_line(block.lines) << std::endl;
    queue->file_queue.queue.pop();
  }
}
