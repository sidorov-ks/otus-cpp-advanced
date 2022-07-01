#include <vector>
#include <list>
#include <functional>
#include <string>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

struct SearchConfiguration {
  std::vector<fs::path> scan_dirs;
  std::vector<fs::path> exclude_dirs;
  unsigned short depth;
  std::size_t min_bytes;
  std::vector<std::string> filename_masks;
  std::size_t block_size;
  std::function<char *(std::string)> hash;
};