#pragma once

#include "../block.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

struct LogQueue {
  std::queue<Block> queue;
  std::mutex mutex;
  std::condition_variable empty_cv;
};

struct FileQueue {
  std::queue<Block> queue;
  bool parity{false}; // Флаг, устанавливающий, какой из двух потоков занимается обработкой нового элемента
  std::mutex mutex;
  std::condition_variable empty_cv; // Уведомляется, когда в очереди появляется элемент
  std::condition_variable parity_cv; // Уведомляется, когда один из потоков закончил обработку и "передал ход" другому
};

std::function<void(const Block &)> make_producer(LogQueue *log_queue, FileQueue *file_queue);