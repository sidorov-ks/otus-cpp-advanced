#include "io.h"
#include "block_processor.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <optional>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int process_args(int argc, char **argv, std::size_t *block_size) {
  po::options_description desc("Allowed options");
  desc.add_options()
          ("help", "produce help message")
          ("block-size", po::value<int>(), "size of a single block");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("block-size")) {
    *block_size = vm["block-size"].as<int>();
    return 0;
  } else {
    std::cout << desc << "\n";
    return 1;
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
