#include <iostream>

#include "block_processor.h"
#include "io.h"

int main(int argc, char **argv) {
  std::size_t block_size;
  auto code = process_args(argc, argv, &block_size);
  if (code != 0) return code;
  BlockProcessor processor(block_size);
  process_stream(processor, std::cin);
  return 0;
}
