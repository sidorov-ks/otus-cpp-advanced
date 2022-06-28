#include "io.h"
#include "block_processor.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <optional>

int process_args(int argc, char **argv, std::size_t *block_size) {
  if (argc == 1) {
    std::cerr << "Usage: ./bulk <block-size>\n";
    return 1;
  }
  try {
    *block_size = std::stol({argv[1]});
    return 0;
  }
  catch (std::invalid_argument const &ex) {
    std::cerr << argv[1] << " is not a number\n";
    return 2;
  }
  catch (std::out_of_range const &ex) {
    std::cerr << argv[1] << " is out of range\n";
    return 3;
  }
}

void process_stream(BlockProcessor &processor, std::istream &in) {
  for (std::string line; std::getline(in, line);) {
    if (line.empty()) continue;
    else if (line == BLOCK_OPEN) {
      processor.open();
    } else if (line == BLOCK_CLOSE) {
      processor.close();
    } else {
      processor.feed(line);
    }
    format_output(processor.get_block());
  }
  processor.halt();
  format_output(processor.get_block());
}

void format_output(const std::optional<Block> &block) {
  if (!block) return;
  std::string output_line = make_output_line(block->lines);
  std::cout << output_line;
  auto filename = "bulk" + std::to_string(block->block_id.time_since_epoch().count());
  std::ofstream fout;
  fout.open(filename);
  fout << output_line;
  fout.flush();
  fout.close();
}

std::string make_output_line(const std::list<std::string> &lines) {
  std::stringstream ss;
  ss << "bulk: ";
  for (auto it = lines.begin(); it != lines.end(); ++it) {
    ss << *it;
    auto it_next = it;
    if (++it_next != lines.end()) ss << ", ";
  }
  ss << "\n";
  return ss.str();
}
