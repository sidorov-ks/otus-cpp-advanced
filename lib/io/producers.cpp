#include <iostream>
#include "producers.h"

BlockProcessor::CallbackType make_producer(std::shared_ptr<ProducerQueues> queues) {
  return [queues](const Block &block) {
    {
      std::unique_lock lock(queues->log_queue.mutex);
      queues->log_queue.queue.push(block);
      queues->log_queue.empty_cv.notify_all();
    }
    bool current_parity = queues->file_parity_switch.fetch_xor(1);
    {
      std::unique_lock lock(queues->file_queue.mutex);
      queues->file_queue.queue.push(std::make_pair(block, current_parity));
      queues->file_queue.empty_cv.notify_all();
    }
  };
}