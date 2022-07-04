#include <iostream>

#include "bayan.h"

int main(int argc, char **argv) {
  auto search_cfg = process_args(argc, argv);
  if (!search_cfg) return -1;
  auto hash = [search_cfg](const std::string &text) { return hashing::hash(search_cfg->hash, text); };
  auto files = collect_files(search_cfg.value());
  auto groups = find_duplicates(files, search_cfg->block_size, hash);
  for (const auto &group: groups) {
    for (const auto &file: group) {
      std::cout << file << " ";
    }
    std::cout << "\n";
  }
  return 0;
}
