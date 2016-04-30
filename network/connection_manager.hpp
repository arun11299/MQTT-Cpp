#ifndef MQTT_CONNECTION_MANAGER_HPP_
#define MQTT_CONNECTION_MANAGER_HPP_

#include <memory>
#include <set>
#include "asio.hpp"
#include "connection.hpp"

namespace mqtt {

class ConnectionManager
{
public:
  ConnectionManager();

  Connection* create_connection(asio::io_context& io_ctxt);

  void remove_connection(Connection* conn);

private:
  using connection_ptr = std::shared_ptr<Connection>;

  std::set<connection_ptr> connections_;
};

};

#endif
