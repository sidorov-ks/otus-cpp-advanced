#pragma once

#include "producers.h"

void log_thread_fn(LogQueue *queue);

void file_thread_fn(FileQueue *queue, bool parity);
