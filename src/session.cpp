#include "session.h"

#include <iostream>

const char BLOCK_OPEN[] = "{";
const char BLOCK_CLOSE[] = "}";

tcp::socket &BlockProcessorSession::socket() {
  return socket_;
}

void BlockProcessorSession::await_message() {
  auto token = [this](const boost::system::error_code &error, std::size_t bytes_transferred) {
    handle_read(error, bytes_transferred);
  };
  socket_.async_read_some(boost::asio::buffer(data_, max_length), token);
}

void BlockProcessorSession::handle_read(const boost::system::error_code &error, std::size_t bytes_transferred) {
  if ((boost::asio::error::eof == error) ||
      (boost::asio::error::connection_reset == error))
  {
    private_.halt();
    private_.send_block();
  }
  else if (!error) {
    std::string line;
    for (std::size_t ix = 0; ix <= bytes_transferred; ++ix) {
      if (ix == bytes_transferred || data_[ix] == '\n') {
        if (line.empty()) continue;
        else if (line == BLOCK_OPEN) {
          private_.open();
        }
        else if (line == BLOCK_CLOSE) {
          private_.close();
          private_.send_block();
        }
        else {
          if (private_.is_dynamic()) {
            private_.feed(line);
            private_.send_block();
          }
          else {
            broadcast_->feed(line);
            broadcast_->send_block();
          }
          line.clear();
        }
      } else {
        line += data_[ix];
      }
    }
  } else {
    delete this;
  }
  await_message();
}