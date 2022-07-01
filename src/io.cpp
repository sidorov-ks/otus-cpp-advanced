#include "io.h"

#include <iostream>
#include <functional>
#include <optional>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace hashing {
enum HashingAlgo {
  MD5, CRC32
};

std::istream &operator>>(std::istream &in, HashingAlgo &algo) {
  std::string token;
  in >> token;
  if (token == "md5")
    algo = MD5;
  else if (token == "crc32")
    algo = CRC32;
  else
    in.setstate(std::ios_base::failbit);
  return in;
}
}

std::optional<SearchConfiguration> process_args(int argc, char **argv) {
  SearchConfiguration config;
  po::options_description desc("Allowed options");
  hashing::HashingAlgo hashing_algorithm;
  desc.add_options()
          ("help", "Produce help message")
          ("scan-dir",
           po::value<std::vector<fs::path>>(&config.scan_dirs)->default_value({fs::current_path()},
                                                                              "current directory"),
           "Directories for running the duplicate search. Defaults to the current directory")
          ("exclude-dir",
           po::value<std::vector<fs::path>>(&config.exclude_dirs),
           "Directories that will be SKIPPED during the duplicate search")
          ("d,depth",
           po::value<unsigned short>(&config.depth)->default_value(0),
           "Scan only files that are nested at most `arg` directories from the scan directories. "
           "Default value corresponds to scanning only files in the directories")
          ("min-bytes",
           po::value<std::size_t>(&config.min_bytes)->default_value(2),
           "Scan only files at least `arg` bytes large")
          ("filename-masks",
           po::value<std::vector<std::string>>(&config.filename_masks)->multitoken(),
           "Scan only files with the name matching one of the masks")
          ("block-size",
           po::value<std::size_t>(&config.block_size)->default_value(128),
           "Scan files in blocks of `arg` bytes")
          ("hash",
           po::value<hashing::HashingAlgo>(&hashing_algorithm)->default_value(hashing::HashingAlgo::MD5, "md5"),
           "Hashing algorithm to use for scanned blocks. Supported values: md5, crc32");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  try {
    po::notify(vm);
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return std::nullopt;
  }

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return std::nullopt;
  }

  return {config};
}
