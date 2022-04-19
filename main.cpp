#include "ip.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>

template<typename InputIterator>
void output_if(InputIterator begin, InputIterator end,
               const std::function<bool(const IPAddress &)> &cond, std::ostream &os) {
  for (auto it = begin; it != end; ++it) {
    if (cond(*it)) os << *it << "\n";
  }
}

int main(int, char **) {
  std::vector<IPAddress> addresses{};
  std::vector<std::string> bad_addresses;
  while (std::cin) {
    std::string _address_line;
    std::getline(std::cin, _address_line);
    std::stringstream ss{_address_line};
    IPAddress _address{};
    std::string _field_2, _field_3;
    ss >> _address >> _field_2 >> _field_3;
    if (!ss.fail()) {
      addresses.push_back(_address);
    } else {
      bad_addresses.push_back(_address_line);
    }
  }
  if (bad_addresses.empty()) {
    std::sort(addresses.begin(), addresses.end(),
              [](IPAddress lhs, IPAddress rhs) { return lhs.address > rhs.address; });
    output_if(addresses.begin(), addresses.end(),
              [](IPAddress address) {
                (void) address;
                return true;
              }, std::cout);
    output_if(addresses.begin(), addresses.end(),
              [](IPAddress address) { return get_octet(address, 1) == 1; },
              std::cout);
    output_if(addresses.begin(), addresses.end(),
              [](IPAddress address) { return get_octet(address, 1) == 46 && get_octet(address, 2) == 70; },
              std::cout);
    output_if(addresses.begin(), addresses.end(),
              [](IPAddress address) {
                unsigned char octets[4] = {get_octet(address, 1),
                                           get_octet(address, 2),
                                           get_octet(address, 3),
                                           get_octet(address, 4)};
                return (octets[0] == 46 || octets[1] == 46 || octets[2] == 46 || octets[3] == 46);
              },
              std::cout);
  } else {
    std::cerr << "Encountered malformed input lines:\n";
    for (const auto &bad_address: bad_addresses) {
      std::cerr << bad_address << "\n";
    }
  }
}
