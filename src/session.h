#pragma once

#include <memory>

#include <boost/asio.hpp>

#include "block_processor.h"

using boost::asio::ip::tcp;

class BlockProcessorSession {
public:
  BlockProcessorSession(boost::asio::io_service &io_service, std::shared_ptr<BlockProcessor> &broadcast,
                        BlockProcessor &&processor);

  void await_message(std::shared_ptr<BlockProcessorSession> session);

  tcp::socket &socket();

private:
  void handle_read(const boost::system::error_code &error, std::size_t bytes_transferred);

  tcp::socket socket_;
  static const std::size_t max_length = 1024;
  char data_[max_length];

  std::shared_ptr<BlockProcessor> broadcast_;
  BlockProcessor private_;
};
