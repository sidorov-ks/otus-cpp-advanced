#pragma once

#include <list>
#include <string>
#include <chrono>

struct Block {
  std::chrono::time_point<std::chrono::system_clock> block_id;
  std::list<std::string> lines;
};
