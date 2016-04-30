#ifndef MQTT_SERVER_HPP_
#define MQTT_SERVER_HPP_

#include "asio.hpp"
#include "io_context_pool.hpp"
#include "connection_manager.hpp"

namespace mqtt {

class TcpServer
{
public:
  TcpServer(const std::string& address,
	    const std::string& port,
	    size_t thread_pool_size);
  void run();
  void stop();
protected:
  void start_accept();

private:
  void handle_accept(const std::error_code& ec);

private:
  IOContextPool io_pool_;
  asio::ip::tcp::acceptor acceptor_;
  Connection* incoming_conn_ = nullptr;
  ConnectionManager connection_mgr_;
};

};
#endif
