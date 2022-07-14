#include "producers.h"

std::function<void(const Block &)> make_producer(LogQueue *log_queue, FileQueue *file_queue) {
  return [log_queue, file_queue](const Block &block) {
    {
      std::unique_lock lock(log_queue->mutex);
      log_queue->queue.push(block);
      log_queue->empty_cv.notify_all();
    }
    {
      std::unique_lock lock(file_queue->mutex);
      file_queue->queue.push(block);
      file_queue->empty_cv.notify_all();
    }
  };
}