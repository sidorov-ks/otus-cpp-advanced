#include <thread>

#include "async.h"

int main(int, char **) {
  async::startup();

  std::size_t bulk = 5;

  std::thread thread{[bulk]() {
    auto h = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
  }};

  std::thread thread2{[bulk]() {
    auto h2 = async::connect(bulk);
    async::receive(h2, "1\n", 2);
    async::disconnect(h2);
  }};

  thread.join();
  thread2.join();

  // TODO stop queue processing threads

  return 0;
}
