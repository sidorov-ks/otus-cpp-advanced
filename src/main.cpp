#include "ip.hpp"

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <tuple>

using namespace std::literals::string_literals;

int main(int, char **) {
  std::cout << output_ip
            << char(-1) << "\n"
            << short(0) << "\n"
            << int(2130706433) << "\n"
            << long(8875824491850138409) << "\n"
            << "8.8.8.8" << "\n"
            << "8.8.8.8"s << "\n"
            << std::vector{127, 0, 0, 1} << "\n"
            << std::list{127, 0, 0, 1} << "\n"
            << std::tuple{127, 0, 0, 1} << "\n";
}
