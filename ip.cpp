#include "ip.h"
#include "version.h"

#include <cassert>

int version() {
  return PROJECT_VERSION_PATCH;
}

unsigned char get_octet(const IPAddress &address, size_t octet) {
  assert(octet >= 1 && octet <= 4);
  switch (octet) {
    case 1:
      return address.address >> 24;
    case 2:
      return (address.address >> 16) & 0xFF;
    case 3:
      return (address.address >> 8) & 0xFF;
    case 4:
      return address.address & 0xFF;
    default:
      return 0;
  }
}

std::istream &operator>>(std::istream &is, IPAddress &ip) {
  std::string token;
  is >> token;
  std::string buffer;
  const int n_octets = 4;
  ip.address = 0;
  for (size_t octet = 1; octet <= n_octets; ++octet) {
    if (token.empty()) {
      is.setstate(std::ios_base::failbit);
    }
    size_t pos = token.find('.');
    if (pos == std::string::npos) {
      pos = token.length();
    }
    buffer = token.substr(0, pos);
    try {
      unsigned char octet_val = static_cast<unsigned char>(std::stoi(buffer));
      ip.address <<= 8;
      ip.address |= octet_val;
      token.erase(0, pos + 1);
    }
    catch (std::invalid_argument const &ex) {
      is.setstate(std::ios_base::failbit);
    }
    catch (std::out_of_range const &ex) {
      is.setstate(std::ios_base::failbit);
    }
    if (is.fail()) break;
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, const IPAddress &ip) {
  return os << static_cast<int>(get_octet(ip, 1)) << "."
            << static_cast<int>(get_octet(ip, 2)) << "."
            << static_cast<int>(get_octet(ip, 3)) << "."
            << static_cast<int>(get_octet(ip, 4));
}
