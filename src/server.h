#pragma once

#include "block_processor.h"
#include "session.h"
#include "io/producers.h"

#include <boost/asio.hpp>

#include <memory>

using boost::asio::ip::tcp;

class BulkServer {
public:
  BulkServer(short port, std::size_t bulk_size);

  void run();

  void setup_session();

private:

  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;
  std::size_t bulk_size_;
  std::shared_ptr<ProducerQueues> queues_;
  std::shared_ptr<BlockProcessor> broadcast_;
  BlockProcessor::CallbackType producer_;
  std::vector<std::thread> consumer_threads_;
};
