#include "block_processor.h"

void BlockProcessor::open() {
  if (!is_dynamic()) {
    flush();
  }
  _balance++;
}

void BlockProcessor::close() {
  if (_balance > 0) _balance--;
  if (!is_dynamic()) {
    flush();
  }
}

void BlockProcessor::feed(const std::string &line) {
  if (!_first_line) _first_line = std::optional<TimePointType>{std::chrono::system_clock::now()};
  _running_block.push_back(line);
  _return_block.clear();
  if (!is_dynamic() && _running_block.size() == _block_size) flush();
}

std::optional<Block> BlockProcessor::get_block() const {
  if (_return_block.empty()) return std::nullopt;
  else {
    assert(_prev_first_line);
    return {{_prev_first_line.value(), _return_block}};
  }
}

bool BlockProcessor::send_block() const {
  auto _block = get_block();
  if (_block) {
    _callback(_block.value());
    return true;
  } else return false;
}

void BlockProcessor::flush() {
  _return_block.clear();
  _return_block.assign(_running_block.begin(), _running_block.end());
  _running_block.clear();
  _prev_first_line = _first_line;
  _first_line.reset();
}

bool BlockProcessor::is_dynamic() const {
  return _balance > 0;
}

void BlockProcessor::halt() {
  if (!is_dynamic()) flush();
}
