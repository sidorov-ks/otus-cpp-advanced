#include <iostream>

#include "io.h"

int main(int argc, char **argv) {
  auto search_cfg = process_args(argc, argv);
  if (!search_cfg) return -1;
  auto files = collect_files(search_cfg.value());
  for (const auto &file: files) std::cout << file << "\n";
  return 0;
}
