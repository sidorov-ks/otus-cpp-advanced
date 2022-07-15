#pragma once

#include "../block.h"
#include "../block_processor.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <memory>

#include <iostream>

struct LogQueue {
  std::queue<Block> queue;
  std::mutex mutex;
  std::condition_variable empty_cv;
};

struct FileQueue {
  std::queue<std::pair<Block, bool>> queue;
  std::mutex mutex;
  std::condition_variable empty_cv;
};

struct ProducerQueues {
  LogQueue log_queue;
  std::atomic<unsigned char> file_parity_switch;
  FileQueue file_queue;
};

BlockProcessor::CallbackType make_producer(std::shared_ptr<ProducerQueues> queues);