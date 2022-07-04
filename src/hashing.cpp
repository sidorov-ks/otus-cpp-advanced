#include "hashing.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

std::istream &operator>>(std::istream &in, hashing::HashingAlgo &algo) {
  std::string token;
  in >> token;
  if (token == "md5")
    algo = hashing::HASH_MD5;
  else if (token == "sha256")
    algo = hashing::HASH_SHA256;
  else
    in.setstate(std::ios_base::failbit);
  return in;
}

constexpr std::size_t MAX_BUFFER_LENGTH = std::max(MD5_DIGEST_LENGTH, SHA256_DIGEST_LENGTH);

std::string hashing::hash(hashing::HashingAlgo algo, const std::string &text) {
  std::size_t buffer_len = 0;
  switch (algo) {
    case HASH_MD5:
      buffer_len = MD5_DIGEST_LENGTH;
      break;
    case HASH_SHA256:
      buffer_len = SHA256_DIGEST_LENGTH;
      break;
  }
  unsigned char hash_buffer[MAX_BUFFER_LENGTH];
  switch (algo) {
    case HASH_MD5:
      MD5(reinterpret_cast<const unsigned char *>(text.c_str()), text.length(), hash_buffer);
      break;
    case HASH_SHA256:
      SHA256(reinterpret_cast<const unsigned char *>(text.c_str()), text.length(), hash_buffer);
      break;
    default:
      return text;
  }
  std::stringstream ss;
  for (std::size_t ix = 0; ix < buffer_len; ++ix) {
    ss << std::hex << (int) hash_buffer[ix];
  }
  auto hash = ss.str();
  return hash;
}