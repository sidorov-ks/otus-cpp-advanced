#include "ip.h"

#include <iostream>
#include <vector>
#include <algorithm>

int main(int, char **) {
  std::vector<IPAddress> addresses{};
  IPAddress _address{};
  while (std::cin >> _address) {
    addresses.push_back(_address);
    std::string _field_2, _field_3;
    std::cin >> _field_2 >> _field_3;
  }
  std::sort(addresses.begin(), addresses.end(),
            [](IPAddress lhs, IPAddress rhs) { return lhs.address > rhs.address; });
  for (const IPAddress &address: addresses)
    std::cout << address << "\n";
  for (const IPAddress &address: addresses) {
    if (get_octet(address, 1) == 1)
      std::cout << address << "\n";
  }
  for (const IPAddress &address: addresses) {
    if (get_octet(address, 1) == 46 && get_octet(address, 2) == 70)
      std::cout << address << "\n";
  }
  for (const IPAddress &address: addresses) {
    unsigned char octets[4] = {get_octet(address, 1),
                               get_octet(address, 2),
                               get_octet(address, 3),
                               get_octet(address, 4)};
    if (octets[0] == 46 || octets[1] == 46 || octets[2] == 46 || octets[3] == 46)
      std::cout << address << "\n";
  }
}
