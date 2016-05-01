#ifndef MQTT_PACKET_HPP
#define MQTT_PACKET_HPP

#include <type_traits>
#include <cstdint>

static const int VARIABLE_HEADER_LIMIT_CASE  = 3;
static const int MAXIMUM_PAYLOAD = 2097151;

namespace mqtt
{

enum MQTTMessageType: uint8_t
{
  RESERVED_1  = 0,  /* Reserved */
  CONNECT     = 1,  /* Client request to connect to Server */
  CONNACK     = 2,  /* Connect Acknowledgment */
  PUBLISH     = 3,  /* Publish message */
  PUBACK      = 4,  /* Publish Acknowledgment */
  PUBREC      = 5,  /* Publish Received (assured delivery part 1) */
  PUBREL      = 6,  /* Publish Release (assured delivery part 2) */
  PUBCOMP     = 7,  /* Publish Complete (assured delivery part 3) */
  SUBSCRIBE   = 8,  /* Client Subscribe request */
  SUBACK      = 9,  /* Subscribe Acknowledgment */
  UNSUBSCRIBE = 10, /* Client Unsubscribe request */
  UNSUBACK    = 11, /* Unsubscribe Acknowledgment */
  PINGREQ     = 12, /* PING Request */
  PINGRESP    = 13, /* PING Response */
  DISCONNECT  = 14, /* Client is Disconnecting */
  RESERVED_2  = 15, /* Reserved */
};

enum MQTTQoS: uint8_t
{
  AT_MOST_ONCE  = 0, /* Fire and Forget */
  AT_LEAST_ONCE = 1, /* Acknowledged delivery */
  EXACTLY_ONCE  = 2, /* Assured delivery */
  RESERVED      = 3,
};

struct MQTTPacketHeaderBase
{
  uint8_t header_[2];
};

struct MQTTVariableHeaderLen
{
  uint8_t variable_len_;
};


template <size_t size,
	 typename = typename std::enable_if<size < VARIABLE_HEADER_LIMIT_CASE>::type>
struct MQTTPacketHeader: MQTTPacketHeader<size - 1>
{
  MQTTVariableHeaderLen hlen;
};

template <>
struct MQTTPacketHeader<0>: MQTTPacketHeaderBase
{
};

/*
MQTTHeaderHelper create_raw_header(char* message_buffer, uint32_t message_len)
{
  assert (message_buffer && message_len <= MAXIMUM_PAYLOAD);

  MQTTHeaderHelper helper(
      *reinterpret_cast<MQTTPacketHeaderBase*>(message_buffer));
  helper.message_length(message_len);

  return helper;
}
*/


class MQTTHeaderHelper
{
public:
  MQTTHeaderHelper(MQTTPacketHeaderBase& hdr);
  MQTTHeaderHelper(const MQTTHeaderHelper&) = delete;
  void operator=(const MQTTHeaderHelper&) = delete;

public:
  bool retain_flag() const noexcept;
  void retain_flag(bool value) noexcept;

  MQTTQoS qos_value() const noexcept;
  void qos_value(MQTTQoS value) noexcept;

  bool dup_flag() const noexcept;
  void dup_flag(bool value) noexcept;

  MQTTMessageType message_type() const noexcept;
  void message_type(MQTTMessageType mtype) noexcept;

  uint32_t message_length() const noexcept;
  void message_length(uint32_t length) noexcept;

private:
  MQTTPacketHeaderBase& pkt_header_;
};

}

#endif
