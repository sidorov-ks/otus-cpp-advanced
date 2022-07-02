#include "io.h"

#include <iostream>
#include <functional>
#include <optional>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

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
          ("depth",
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
    BOOST_LOG_TRIVIAL(fatal) << e.what();
    return std::nullopt;
  }

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return std::nullopt;
  }

  return {config};
}

std::vector <fs::path> collect_files(const SearchConfiguration &config, unsigned short depth = 0);

std::vector <fs::path> collect_files(const SearchConfiguration &config) {
  return collect_files(config, 0);
}

// HACK make boost do it automagically
#define FORMAT_DEPTH(depth, config) "[Depth " << (depth) << "/" << ((config).depth) << "] "

std::vector <fs::path> collect_files(const SearchConfiguration &config, unsigned short depth) {
  std::vector <fs::path> res;
  std::vector <fs::path> subdirs;
  BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Collecting a list of files for the duplicate scan";
  for (const auto &dir: config.scan_dirs) {
    if (fs::is_directory(dir)) {
      bool is_excl = false;
      for (const auto &excl_dir: config.exclude_dirs) {
        if (fs::equivalent(dir, excl_dir)) {
          BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Directory " << dir
                                  << " found in exclude list, ignoring it";
          is_excl = true;
          break;
        }
      }
      if (is_excl) continue;
      BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Collecting files from the directory " << dir;
      for (auto it = fs::directory_iterator(dir); it != fs::directory_iterator(); ++it) {
        auto entry = *it;
        auto status = entry.status();
        auto path = entry.path();
        if (status.type() == fs::directory_file) {
          if (depth < config.depth) {
            BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config)
                                    << "Encountered directory " << entry << ", adding to the recursive scan";
            subdirs.push_back(path);
          } else {
            BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Ignoring directory " << entry;
          }
        } else if (status.type() == fs::regular_file) {
          if (fs::file_size(path) >= config.min_bytes) {
            BOOST_LOG_TRIVIAL(trace) << FORMAT_DEPTH(depth, config) << "File " << entry << " is too small, skipping";
          } else {
            BOOST_LOG_TRIVIAL(trace) << FORMAT_DEPTH(depth, config) << "Adding file " << entry << " to the scan";
            res.push_back(path);
          }
        } else {
          BOOST_LOG_TRIVIAL(trace) << FORMAT_DEPTH(depth, config) << "Ignoring file " << entry
                                   << " with unknown status " << status.type();
        }
      }
    } else {
      BOOST_LOG_TRIVIAL(warning) << FORMAT_DEPTH(depth, config) << "Cannot scan files in " << dir
                                 << " as it is not a directory, skipping...";
    }
  }
  if (depth < config.depth) {
    BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Encountered "
                            << res.size() << " files and "
                            << subdirs.size() << " subdirectories, running file search on them";
    SearchConfiguration new_config = config;
    new_config.scan_dirs = subdirs;
    auto subdir_files = collect_files(new_config, depth + 1);
    res.insert(res.end(), std::make_move_iterator(subdir_files.begin()), std::make_move_iterator(subdir_files.end()));
  }
  BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Found " << res.size()
                          << " files in all scanned directories";
  return res;
}