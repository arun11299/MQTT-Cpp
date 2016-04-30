#ifndef MQTT_SERVER_HPP_
#define MQTT_SERVER_HPP_

#include "asio.hpp"
#include "io_context_pool.hpp"
#include "connection_manager.hpp"

namespace mqtt {

class TcpServer
{
public:
  TcpServer();
  void run();
  void stop();
protected:
  void start_accept();

private:
  IOContextPool io_pool_;
  asio::ip::tcp::acceptor acceptor_;
  Connection* incoming_conn_ = nullptr;
  ConnectionMgr connection_mgr_;
};

};
#endif
