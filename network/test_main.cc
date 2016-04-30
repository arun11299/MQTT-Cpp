#include <iostream>
#include "server.hpp"

using namespace mqtt;

int main() {
  TcpServer("localhost", "8888", 1).run();
  return 0;
}
