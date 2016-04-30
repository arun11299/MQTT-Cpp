#include <iostream>
#include <string>
#include "server.hpp"
#include "connection.hpp"

using namespace mqtt;

TcpServer::TcpServer(const std::string& address,
		     const std::string& port,
		     size_t thread_pool_size):
  io_pool_(thread_pool_size),
  acceptor_(io_pool_.get_io_context())
{
  std::error_code ec;

  asio::ip::tcp::resolver resolver(
      acceptor_.get_io_service());

  asio::ip::tcp::endpoint endpoint = 
    *resolver.resolve(address, port).begin();

  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));

  acceptor_.bind(endpoint, ec);
  if (ec) {
    std::cerr << ec.message() << std::endl;
    ec.clear();
    return;
  }

  acceptor_.listen(asio::socket_base::max_connections, ec);
  if (ec) {
    std::cerr << ec.message() << std::endl;
    ec.clear();
    return;
  }

  start_accept();
}

void TcpServer::run()
{
  io_pool_.run();
}

void TcpServer::stop()
{
  io_pool_.stop();
}

void TcpServer::start_accept()
{
  incoming_conn_ = connection_mgr_.create_connection(
      io_pool_.get_io_context());

  acceptor_.async_accept(incoming_conn_->socket(),
      std::bind(&TcpServer::handle_accept, this, std::placeholders::_1));
}

void TcpServer::handle_accept(const std::error_code& e)
{
  assert (incoming_conn_);
  if (!e) {
    incoming_conn_->start_streaming();
  }

  start_accept();
}
