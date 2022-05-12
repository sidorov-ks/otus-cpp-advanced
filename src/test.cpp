#include <gtest/gtest.h>

#include "ip.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <tuple>

using namespace std::literals::string_literals;

TEST(ip_octet_write, write_string) {
  std::stringstream ss;
  auto ip_addr = "127.0.0.1"s;
  ss << output_ip << ip_addr;
  EXPECT_EQ(ip_addr, ss.str());
}

TEST(ip_octet_write, write_cstring) {
  std::stringstream ss;
  auto ip_addr = "127.0.0.1";
  ss << output_ip << ip_addr;
  EXPECT_EQ(ip_addr, ss.str());
}

TEST(ip_octet_write, write_str_literal) {
  std::stringstream ss;
  ss << output_ip << "127.0.0.1";
  EXPECT_EQ("127.0.0.1", ss.str());
}

TEST(ip_octet_write, write_char) {
  std::stringstream ss;
  ss << output_ip << char(-1);
  EXPECT_EQ("255.255.255.255", ss.str());
}

TEST(ip_octet_write, write_int) {
  std::stringstream ss;
  ss << output_ip << int(2130706433);
  EXPECT_EQ("127.0.0.1", ss.str());
}

TEST(ip_octet_write, write_vector) {
  std::stringstream ss;
  ss << output_ip << std::vector{1, 2, 3, 4};
  EXPECT_EQ("1.2.3.4", ss.str());
}

TEST(ip_octet_write, write_list) {
  std::stringstream ss;
  ss << output_ip << std::list{1, 2, 3, 4};
  EXPECT_EQ("1.2.3.4", ss.str());
}

TEST(ip_octet_write, write_tuple) {
  std::stringstream ss;
  ss << output_ip << std::tuple{1, 2, 3, 4};
  EXPECT_EQ("1.2.3.4", ss.str());
}