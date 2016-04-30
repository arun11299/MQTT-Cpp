#ifndef MQTT_CONNECTION_HPP_
#define MQTT_CONNECTION_HPP_

#include <memory>
#include "asio.hpp"
#include "connection_manager.hpp"

namespace mqtt {

class Connection: 
  public std::enable_shared_from_this<Connection>
{
public:
  Connection(asio::io_context& io_ctxt,
	     ConnectionManager& conn_mgr);

  asio::ip::tcp::socket& socket() noexcept;

  void start_streaming();

  void close();
private:
  void handle_read();
  void handle_write();

private:
  asio::ip::tcp::socket socket_;

  ConnectionManager& connection_mgr_;
};

};

#endif
