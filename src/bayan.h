#pragma once

#include <optional>
#include <vector>
#include <regex>

#include <boost/filesystem.hpp>

#include "search_config.h"

std::regex make_regex(const std::string &glob);

std::optional<SearchConfiguration> process_args(int, char **);

std::vector<boost::filesystem::path> collect_files(const SearchConfiguration &);

std::vector<std::vector<boost::filesystem::path>>
find_duplicates(const std::vector<boost::filesystem::path> &, std::size_t, const HashFn &);