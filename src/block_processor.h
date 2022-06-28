#pragma once

#include <cassert>
#include <chrono>
#include <optional>
#include <list>

#include "block.h"

class BlockProcessor {
public:
  using BlockType = std::list<std::string>;
  using TimePointType = std::chrono::time_point<std::chrono::system_clock>;

  explicit BlockProcessor(std::size_t block_size) : _block_size(block_size), _balance(0) {}

  void open();

  void close();

  void feed(const std::string &line);

  [[nodiscard]] std::optional<Block> get_block() const;

  void halt();

private:
  void flush();

  [[nodiscard]] bool is_dynamic() const;

  std::size_t _block_size;
  std::size_t _balance;
  BlockType _running_block;
  BlockType _return_block;
  std::optional<TimePointType> _first_line;
  std::optional<TimePointType> _prev_first_line;
};
