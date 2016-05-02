#include "mqtt_packet.hpp"
#ifdef NDEBUG
#include <iostream>
#endif

using namespace mqtt;

MQTTHeaderHelper::MQTTHeaderHelper(
    MQTTPacketHeaderBase& header): pkt_header_(header)
{
}

bool MQTTHeaderHelper::retain_flag() const noexcept
{
  return pkt_header_.header_[0] & 0x01;
}

void MQTTHeaderHelper::retain_flag(bool value) noexcept
{
  if (value) pkt_header_.header_[0] |= 0x01;
  else pkt_header_.header_[0] &= 0xFE;
}

MQTTQoS MQTTHeaderHelper::qos_level() const noexcept
{
  return static_cast<MQTTQoS>((pkt_header_.header_[0] >> 1) & 0x02);
}

void MQTTHeaderHelper::qos_level(MQTTQoS value) noexcept
{
  pkt_header_.header_[0] |= (value << 1);
}

bool MQTTHeaderHelper::dup_flag() const noexcept
{
  return (pkt_header_.header_[0] >> 3) & 0x01;
}

void MQTTHeaderHelper::dup_flag(bool value) noexcept
{
  if (value) pkt_header_.header_[0] |= 0x08;
  else pkt_header_.header_[0] &= 0xF7;
}

MQTTMessageType MQTTHeaderHelper::message_type() const noexcept
{
  return static_cast<MQTTMessageType>((pkt_header_.header_[0] >> 4) & 0x0F);
}

void MQTTHeaderHelper::message_type(MQTTMessageType mtype) noexcept
{
  pkt_header_.header_[0] |= (mtype << 4);
}

uint32_t MQTTHeaderHelper::message_length() const noexcept
{
  int length = 0;
  int multiplier = 1;
  uint8_t* len_buf = &pkt_header_.header_[1];

  for (int i = 0; i < VARIABLE_HEADER_LIMIT_CASE; i++) {
    length = (*len_buf & 0x7F) * multiplier;
    multiplier *= 128;
    if (!( (*len_buf & 0x80) >> 7)) break;
    len_buf++;
  }
  return length;
}

void MQTTHeaderHelper::message_length(uint32_t length) noexcept
{
  uint8_t* len_buf = &pkt_header_.header_[1];
  while (length) {
    uint8_t digit = length % 128;
    length /= 128;
    if (length) digit |= 0x80;
    *len_buf = digit;
    len_buf++;
  }
}

#ifdef NDEBUG
void MQTTHeaderHelper::print_header()
{
  uint8_t* buf = &pkt_header_.header_[0];

  auto print_helper = [](uint8_t* buf) {
    uint8_t mask = 0x80;
    int shift = 7;
    while (mask) {
      uint8_t v = (*buf & mask) >> shift;
      if (v) std::cout << "| 1 ";
      else std::cout << "| 0 ";
      shift--;
      mask >>= 1;
    }
    std::cout << std::endl;
  };
  // print 1st byte of header
  print_helper(buf);
  buf++;
  //print second byte of header
  print_helper(buf);
}
#endif
