#include <gtest/gtest.h>

#include <list>
#include <string>
#include <optional>

#include "block_processor.h"

const std::size_t BLOCK_SIZE = 4;
const auto NULL_CALLBACK = [](const Block &_block) { (void) _block; };

TEST(block_processor, static_block_single) {
  BlockProcessor processor{BLOCK_SIZE, NULL_CALLBACK};
  std::list<std::string> lines{"ab", "ba", "aca", "bacca"};
  ASSERT_EQ(BLOCK_SIZE, lines.size());
  for (const auto &line: lines) {
    EXPECT_FALSE(processor.get_block().has_value());
    processor.feed(line);
  }
  EXPECT_TRUE(processor.get_block().has_value());
  EXPECT_EQ(processor.get_block().value().lines, lines);
}

TEST(block_processor, static_block_multiple_blocks) {
  BlockProcessor processor{BLOCK_SIZE, NULL_CALLBACK};
  std::list<std::string> lines_first{"ab", "ba", "aca", "bacca"};
  std::list<std::string> lines_second{"ba", "ab", "abba", "cabba"};
  std::list<std::string> lines_final{"halt", "this", "test"};
  ASSERT_EQ(BLOCK_SIZE, lines_first.size());
  ASSERT_EQ(BLOCK_SIZE, lines_second.size());
  ASSERT_GT(BLOCK_SIZE, lines_final.size());
  for (const auto &line: lines_first) {
    EXPECT_FALSE(processor.get_block().has_value());
    processor.feed(line);
  }
  EXPECT_TRUE(processor.get_block().has_value());
  EXPECT_EQ(processor.get_block().value().lines, lines_first);
  for (auto it = lines_second.begin(); it != lines_second.end(); ++it) {
    if (it != lines_second.begin()) {
      EXPECT_FALSE(processor.get_block().has_value());
    }
    processor.feed(*it);
  }
  EXPECT_TRUE(processor.get_block().has_value());
  EXPECT_EQ(processor.get_block().value().lines, lines_second);
  for (auto it = lines_final.begin(); it != lines_final.end(); ++it) {
    if (it != lines_final.begin()) {
      EXPECT_FALSE(processor.get_block().has_value());
    }
    processor.feed(*it);
  }
  processor.halt();
  EXPECT_TRUE(processor.get_block().has_value());
  EXPECT_EQ(processor.get_block().value().lines, lines_final);
}

TEST(block_processor, dynamic_block_single) {
  BlockProcessor processor{BLOCK_SIZE, NULL_CALLBACK};
  std::list<std::string> lines_start{"static", "block"};
  std::list<std::string> lines_dyn{"a", "long", "dynamic", "block", "because", "we", "can"};
  ASSERT_GT(BLOCK_SIZE, lines_start.size());
  ASSERT_NE(BLOCK_SIZE, lines_dyn.size());
  for (const auto &line: lines_start) {
    EXPECT_FALSE(processor.get_block().has_value());
    EXPECT_FALSE(processor.is_dynamic());
    processor.feed(line);
  }
  EXPECT_FALSE(processor.get_block().has_value());
  EXPECT_FALSE(processor.is_dynamic());
  processor.open();
  EXPECT_TRUE(processor.get_block().has_value());
  EXPECT_TRUE(processor.is_dynamic());
  EXPECT_EQ(processor.get_block().value().lines, lines_start);
  for (auto it = lines_dyn.begin(); it != lines_dyn.end(); ++it) {
    if (it != lines_dyn.begin()) {
      EXPECT_FALSE(processor.get_block().has_value());
    }
    EXPECT_TRUE(processor.is_dynamic());
    processor.feed(*it);
  }
  EXPECT_FALSE(processor.get_block().has_value());
  EXPECT_TRUE(processor.is_dynamic());
  processor.close();
  EXPECT_TRUE(processor.get_block().has_value());
  EXPECT_FALSE(processor.is_dynamic());
  EXPECT_EQ(processor.get_block().value().lines, lines_dyn);
}

TEST(block_processor, dynamic_block_nested) {
  BlockProcessor processor{BLOCK_SIZE, NULL_CALLBACK};
  std::list<std::string> lines{"a", "long", "dynamic", "block", "because", "we", "can"};
  ASSERT_NE(BLOCK_SIZE, lines.size());
  processor.open();
  processor.open();
  for (const auto &line: lines) {
    EXPECT_FALSE(processor.get_block().has_value());
    EXPECT_TRUE(processor.is_dynamic());
    processor.feed(line);
  }
  EXPECT_FALSE(processor.get_block().has_value());
  EXPECT_TRUE(processor.is_dynamic());
  processor.close();
  EXPECT_FALSE(processor.get_block().has_value());
  EXPECT_TRUE(processor.is_dynamic());
  processor.close();
  EXPECT_TRUE(processor.get_block().has_value());
  EXPECT_FALSE(processor.is_dynamic());
  EXPECT_EQ(processor.get_block().value().lines, lines);
}

TEST(block_processor, dynamic_block_interrupted) {
  BlockProcessor processor{BLOCK_SIZE, NULL_CALLBACK};
  std::list<std::string> lines{"a", "long", "dynamic", "block", "because", "we", "can"};
  ASSERT_NE(BLOCK_SIZE, lines.size());
  processor.open();
  for (const auto &line: lines) {
    EXPECT_FALSE(processor.get_block().has_value());
    processor.feed(line);
  }
  EXPECT_FALSE(processor.get_block().has_value());
  processor.halt();
  EXPECT_FALSE(processor.get_block().has_value());
}
