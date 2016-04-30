#include <algorithm>
#include "connection_manager.hpp"
#include "connection.hpp"

using namespace mqtt;

ConnectionManager::ConnectionManager()
{
}

Connection* ConnectionManager::create_connection(
    asio::io_context& io_context)
{
  auto connection(std::make_shared<Connection>(io_context, *this));
  auto* raw_conn = connection.get();
  connections_.insert(connection);
  
  return raw_conn;
}

void ConnectionManager::remove_connection(Connection* connection)
{
  assert (connection);
  auto it = std::find_if(connections_.begin(), connections_.end(),
      [connection](const connection_ptr& e) {
	return connection == e.get();
      });
  assert (it != connections_.end());
  connections_.erase(it);
}
