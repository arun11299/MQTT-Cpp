#include <iostream>
#include "server.hpp"

using namespace mqtt;

int main() {
  TcpServer("10.110.0.23", "8888", 1).run();
  return 0;
}
