#pragma once

#include <memory>

#include "producers.h"

void log_thread_fn(std::shared_ptr<ProducerQueues> queue);

void file_thread_fn(std::shared_ptr<ProducerQueues> queue, bool parity);
