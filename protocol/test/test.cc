#include <iostream>
#include "../mqtt_packet.hpp"

using namespace mqtt;

int main() {
  auto class_type = mqtt_header_class_type(10);
  switch (class_type) {
  case REGULAR: {
      MQTTPacketHeader<REGULAR> hdr;
      MQTTHeaderHelper hlpr(hdr);
      hlpr.message_length(127);
      std::cout << hlpr.message_length() << std::endl;
      hlpr.message_type(CONNECT);
      hlpr.qos_level(EXACTLY_ONCE);
      hlpr.dup_flag(true);
      hlpr.print_header();
  }
    break;
  case MEDIUM:
    break;
  case HIGH:
    break;
  default:
    assert (0);
  };
  return 0;
}
