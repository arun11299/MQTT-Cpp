#include "connection.hpp"

using namespace mqtt;

Connection::Connection(
    asio::io_context& io_context,
    ConnectionManager& conn_mgr):
  socket_(io_context),
  connection_mgr_(conn_mgr)
{
}

asio::ip::tcp::socket& Connection::socket() noexcept
{
  return socket_;
}

void Connection::close()
{
  socket_.cancel();
  socket_.close();
  connection_mgr_.remove_connection(this);
}
