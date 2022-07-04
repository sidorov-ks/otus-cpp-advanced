#pragma once

#include <string>
#include <iostream>

namespace hashing {
enum HashingAlgo {
  HASH_MD5, HASH_SHA256
};

std::string hash(hashing::HashingAlgo algo, const std::string &text);
}

std::istream &operator>>(std::istream &in, hashing::HashingAlgo &algo);
