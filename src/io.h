#pragma once

#include <optional>
#include <vector>

#include <boost/filesystem.hpp>

#include "search_config.h"

std::optional<SearchConfiguration> process_args(int, char **);

std::vector<boost::filesystem::path> collect_files(const SearchConfiguration &);