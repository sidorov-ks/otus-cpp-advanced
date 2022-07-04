#include "bayan.h"
#include "hashing.h"

#include <iostream>
#include <functional>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <memory>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/log/trivial.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

// Собирает регулярное выражение из маски. Преобразование к синтаксису регулярных выражений:
// - Точки в строке экранируются: . -> \.
// - Символ * транслируется как "любое количество любых символов": * -> .*
// - Символ ? транслируется как "любой символ": ? -> .
// - Строка обрамляется символами ^ и $ (начало и окончание строки)
std::regex make_regex(const std::string &glob) {
  std::string mask = std::regex_replace(glob, std::regex{"\\."}, "\\.");
  mask = std::regex_replace(mask, std::regex{"\\*"}, ".*");
  mask = std::regex_replace(mask, std::regex{"\\?"}, ".");
  auto regex = std::regex{"^" + mask + "$"};
  return regex;
}

std::optional<SearchConfiguration> process_args(int argc, char **argv) {
  SearchConfiguration config;
  po::options_description desc("Allowed options");
  std::vector<std::string> filename_masks;
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
           po::value<std::vector<std::string>>(&filename_masks)
                   ->multitoken()
                   ->default_value({"*"}, "no constraints on filenames"),
           "Scan only files with the name matching one of the masks")
          ("block-size",
           po::value<std::size_t>(&config.block_size)->default_value(128),
           "Scan files in blocks of `arg` bytes")
          ("hash",
           po::value<hashing::HashingAlgo>(&config.hash)->default_value(hashing::HashingAlgo::HASH_MD5, "md5"),
           "Hashing algorithm to use for scanned blocks. Supported values: md5, sha256");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  try {
    po::notify(vm);
    for (const auto &mask: filename_masks) {
      config.filename_masks.push_back(make_regex(mask));
    }
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

#define FORMAT_DEPTH(depth, config) "[Depth " << (depth) << "/" << ((config).depth) << "] "

std::vector<fs::path> collect_files(const SearchConfiguration &config) {
  std::vector<fs::path> res;
  std::vector<fs::path> subdirs{config.scan_dirs};
  for (std::size_t depth = 0; depth <= config.depth; ++depth) {
    BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Collecting a list of files for the duplicate scan";
    std::vector<fs::path> new_subdirs;
    std::size_t new_files = 0;
    for (const auto &dir: subdirs) {
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
              new_subdirs.push_back(path);
            } else {
              BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Ignoring directory " << entry;
            }
          } else if (status.type() == fs::regular_file) {
            bool satisfies_mask = false;
            for (const auto &mask: config.filename_masks) {
              if (std::regex_search(path.filename().string(), mask)) {
                satisfies_mask = true;
                break;
              }
            }
            if (!satisfies_mask) {
              BOOST_LOG_TRIVIAL(trace) << FORMAT_DEPTH(depth, config) << "File " << entry
                                       << " does not match the filename masks";
              continue;
            }
            if (fs::file_size(path) < config.min_bytes) {
              BOOST_LOG_TRIVIAL(trace) << FORMAT_DEPTH(depth, config) << "File " << entry << " is too small, skipping";
            } else {
              BOOST_LOG_TRIVIAL(trace) << FORMAT_DEPTH(depth, config) << "Adding file " << entry << " to the scan";
              res.push_back(path);
              new_files++;
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
                              << new_files << " new files and "
                              << new_subdirs.size() << " subdirectories, running file search on them";
      subdirs = new_subdirs;
    } else {
      BOOST_LOG_TRIVIAL(info) << FORMAT_DEPTH(depth, config) << "Encountered "
                              << new_files << " new files";
    }
  }
  BOOST_LOG_TRIVIAL(info) << "Found " << res.size() << " files in all scanned directories";
  return res;
}

using FileGroup = std::vector<fs::path>;

std::vector<FileGroup>
find_duplicates(const FileGroup &files, std::size_t block_size, const HashFn &hash) {
  std::vector<FileGroup> groups{{files}};
  std::vector<FileGroup> new_groups;
  std::unordered_map<std::string, std::shared_ptr<fs::ifstream>> streams;
  std::unordered_map<std::string, fs::path> paths;
  for (const auto &file: files) {
    paths[file.string()] = file;
    streams[file.string()] = std::make_shared<fs::ifstream>();
    streams[file.string()]->open(file);
  }
  std::string buffer(block_size, '\0');
  for (std::size_t block = 1;; ++block) {
    if (streams.empty()) {
      BOOST_LOG_TRIVIAL(info) << "No more files to scan, terminating";
      break;
    }
    new_groups.clear();
    for (const auto &group: groups) {
      if (group.size() == 1) {
        new_groups.push_back({group});
        continue;
      }
      std::unordered_map<std::string, FileGroup> hash_groups;
      std::vector<std::string> deleted_keys;
      for (const auto &path: group) {
        std::fill(buffer.begin(), buffer.end(), '\0');
        const auto &path_str = path.string();
        if (streams.find(path_str) != streams.end()) {
          auto stream = streams[path_str];
          stream->read(buffer.data(), block_size);
          if (stream->eof()) {
            deleted_keys.push_back(path_str);
          }
        }
        std::string hash_value = hash(buffer);
        if (hash_groups.find(hash_value) != hash_groups.end()) {
          hash_groups[hash_value].push_back(paths[path_str]);
        } else {
          hash_groups[hash_value] = {paths[path_str]};
        }
      }
      for (const auto &key: deleted_keys) {
        BOOST_LOG_TRIVIAL(info) << "File " << paths[key] << " has been exhausted after " << block << " blocks";
        streams.erase(key);
      }
      for (const auto &[_, hash_group]: hash_groups) {
        new_groups.push_back(hash_group);
        if (hash_group.size() == 1) {
          auto unique_file = *hash_group.begin();
          BOOST_LOG_TRIVIAL(info) << "File " << unique_file
                                  << " has been identified as unique after " << block
                                  << " blocks, not scanning it any further";
          streams.erase(unique_file.string());
        }
      }
    }
    groups = new_groups;
  }
  return groups;
}