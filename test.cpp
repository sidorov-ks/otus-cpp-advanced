#include <gtest/gtest.h>

#include "ip.h"

TEST(ip_octet_read, test_octet_layout) {
  IPAddress address{0xC0A80001}; // 192.168.0.1
  EXPECT_EQ(get_octet(address, 1), static_cast<unsigned char>(192));
  EXPECT_EQ(get_octet(address, 2), static_cast<unsigned char>(168));
  EXPECT_EQ(get_octet(address, 3), static_cast<unsigned char>(0));
  EXPECT_EQ(get_octet(address, 4), static_cast<unsigned char>(1));
}

TEST(ip_octet_read, test_octet_range) {
  IPAddress address{};
  EXPECT_DEATH({ get_octet(address, -1); }, "Assertion `octet >= 1 && octet <= 4' failed.");
  EXPECT_DEATH({ get_octet(address, 0); }, "Assertion `octet >= 1 && octet <= 4' failed.");
  EXPECT_DEATH({ get_octet(address, 5); }, "Assertion `octet >= 1 && octet <= 4' failed.");
  EXPECT_DEATH({ get_octet(address, 1 << 8); }, "Assertion `octet >= 1 && octet <= 4' failed.");
}

TEST(ip_istream, test_ok_read) {
  IPAddress address{};
  std::stringstream ss{"192.168.0.1"};
  ss >> address;
  EXPECT_EQ(address.address, 0xC0A80001);
}

TEST(ip_istream, test_bad_read) {
  std::string malformed[]{"192.168.0",
                          "a.b.c.d",
                          "abaca",
                          "192 168 0 1"};
  for (const std::string &bad_ip: malformed) {
    IPAddress address{};
    std::stringstream ss{bad_ip};
    ss >> address;
    EXPECT_TRUE(ss.fail()) << "Failed to set `failbit` on string " << bad_ip;
  }
}

TEST(ip_ostream, test_output) {
  std::stringstream ss{};
  ss << IPAddress{0xC0A80001} << "\n" << IPAddress{0xFFFFFFFF} << "\n";
  EXPECT_EQ(ss.str(), "192.168.0.1\n255.255.255.255\n");
}