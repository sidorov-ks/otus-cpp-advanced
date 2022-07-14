#include "consumers.h"

#include <mutex>
#include <iostream>
#include <fstream>

std::string make_line(const std::list<std::string> &lines) {
  std::stringstream ss;
  ss << "bulk: ";
  for (auto it = lines.begin(); it != lines.end(); ++it) {
    ss << *it;
    auto it_next = it;
    if (++it_next != lines.end()) ss << ", ";
  }
  ss << "\n";
  return ss.str();
}

void log_thread_fn(LogQueue *queue) {
  while (true) {
    std::unique_lock lock(queue->mutex);
    queue->empty_cv.wait(lock, [&queue]() { return !queue->queue.empty(); });
    const Block &block = queue->queue.front();
    std::cout << make_line(block.lines);
    queue->queue.pop();
  }
}

void file_thread_fn(FileQueue *queue, bool parity) {
  while (true) {
    std::unique_lock lock(queue->mutex);
    queue->parity_cv.wait(lock, [&queue, parity]() { return queue->parity == parity; });
    queue->empty_cv.wait(lock, [&queue]() { return !queue->queue.empty(); });
    queue->parity = !parity;
    const Block &block = queue->queue.front();
    auto filename = "bulk" + std::to_string(block.block_id.time_since_epoch().count()) + "_" + (parity ? "1" : "0");
    std::ofstream fout(filename);
    fout << make_line(block.lines);
    queue->queue.pop();
    queue->parity_cv.notify_all();
  }
}