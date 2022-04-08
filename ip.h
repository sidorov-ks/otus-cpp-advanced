#pragma once

#include <iostream>

int version();

struct IPAddress {
  unsigned int address;
};

unsigned char get_octet(const IPAddress &address, size_t octet);

std::istream &operator>>(std::istream &is, IPAddress &ip);

std::ostream &operator<<(std::ostream &os, const IPAddress &ip);