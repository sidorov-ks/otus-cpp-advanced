#pragma once

#include <cassert>
#include <chrono>
#include <optional>
#include <list>
#include <functional>

#include "block.h"

class BlockProcessor {
public:
  using BlockType = std::list<std::string>;
  using TimePointType = std::chrono::time_point<std::chrono::system_clock>;
  using CallbackType = std::function<void(const Block &)>;

  explicit BlockProcessor(std::size_t block_size, CallbackType callback) : _block_size(block_size),
                                                                           _balance(0), _callback(callback) {}

  void open();

  void close();

  void feed(const std::string &line);

  [[nodiscard]] std::optional<Block> get_block() const;

  bool send_block() const;

  void halt();

  [[nodiscard]] bool is_dynamic() const;
private:
  void flush();

  std::size_t _block_size;
  std::size_t _balance;
  BlockType _running_block;
  BlockType _return_block;
  std::optional<TimePointType> _first_line;
  std::optional<TimePointType> _prev_first_line;
  CallbackType _callback;
};
