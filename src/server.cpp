#include <memory>
#include <atomic>

#include "server.h"
#include "io/producers.h"
#include "io/consumers.h"

BulkServer::BulkServer(short port, std::size_t bulk_size)
        : io_service_(),
          acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
          bulk_size_(bulk_size),
          queues_(new ProducerQueues{LogQueue{}, std::atomic<unsigned char>{false}, FileQueue{}}) {
  consumer_threads_.emplace_back(log_thread_fn, queues_);
  consumer_threads_.emplace_back(file_thread_fn, queues_, true);
  consumer_threads_.emplace_back(file_thread_fn, queues_, false);
  producer_ = make_producer(queues_);
  broadcast_ = std::make_shared<BlockProcessor>(bulk_size_, producer_);
  setup_session();
}

void BulkServer::run() {
  io_service_.run();
}

void BulkServer::setup_session() {
  BlockProcessor session_proc{bulk_size_, producer_};
  std::shared_ptr<BlockProcessorSession> new_session {
    new BlockProcessorSession(io_service_, broadcast_, std::move(session_proc))
  };
  auto token = [new_session, this](const boost::system::error_code &error) {
    if (!error) new_session->await_message(new_session);
    setup_session();
  };
  acceptor_.async_accept(new_session->socket(), token);
}
