#pragma once

#include <vector>
#include <list>
#include <functional>
#include <string>
#include <regex>

#include <boost/filesystem.hpp>

#include "hashing.h"

namespace fs = boost::filesystem;

using HashFn = std::function<std::string(const std::string &)>;

struct SearchConfiguration {
  std::vector<fs::path> scan_dirs;
  std::vector<fs::path> exclude_dirs;
  unsigned short depth;
  std::size_t min_bytes;
  std::vector<std::regex> filename_masks;
  std::size_t block_size;
  hashing::HashingAlgo hash;
};