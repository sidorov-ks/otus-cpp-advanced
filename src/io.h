#pragma once

#include <optional>

#include "search_config.h"

std::optional<SearchConfiguration> process_args(int argc, char **argv);
