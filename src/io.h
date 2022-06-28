#pragma once

#include "block.h"
#include "block_processor.h"

const char BLOCK_OPEN[] = "{";
const char BLOCK_CLOSE[] = "}";

int process_args(int argc, char **argv, std::size_t *block_size);

void process_stream(BlockProcessor &processor, std::istream &in);

void format_output(const std::optional<Block> &block);

std::string make_output_line(const std::list<std::string> &lines);