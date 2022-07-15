#pragma once

#include <list>
#include <string>
#include <chrono>

struct Block {
  std::chrono::time_point<std::chrono::system_clock> block_id;
  std::list<std::string> lines;

  Block(std::chrono::time_point<std::chrono::system_clock> _block_id, std::list<std::string> _lines) : block_id(
          _block_id) {
    for (const auto &line: _lines) lines.push_back(line);
  }

  Block(const Block &block) : block_id(block.block_id) {
    for (const auto &line: block.lines) lines.push_back(line);
  }
};
