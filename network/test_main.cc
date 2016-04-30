#include <iostream>
#include "server.hpp"

using namespace mqtt;

int main() {
  TcpServer("0.0.0.0", "8888", 1).run();
  return 0;
}
