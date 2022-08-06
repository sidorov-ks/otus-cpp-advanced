#include <iostream>

#include "server.h"

int main(int argc, char **argv) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
      return 1;
    }
    short port = std::atoi(argv[1]);
    std::size_t bulk_size = std::atol(argv[2]);
    BulkServer {port, bulk_size}.run();
  }
  catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
