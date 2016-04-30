#include <iostream>
#include <string>
#include "server.hpp"

using namespace mqtt;

TcpServer::TcpServer(const std::string& address,
		       const std::string& port,
		       size_t thread_pool_size):
  io_pool_(thread_pool_size)
{
  asio::ip::tcp::resolver resolver(
      acceptor_.get_executer().context());

  asio::ip::tcp::endpoint = 
    *resolver.resolve(address, port).begin();

  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

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
  new_conn_ = connection_mgr_->create_connection(
      io_pool_.get_io_context(), connection_mgr_);

  acceptor_.async_accept(new_conn_->socket(),
      std::bind(&TcpServer::handle_accept, this, asio::placeholders::error));
}

void TcpServer::handle_accept(asio::error_code& e)
{
  assert (new_conn_);
  if (!e) {
    new_conn_->start_streaming();
  }

  start_accept();
}
