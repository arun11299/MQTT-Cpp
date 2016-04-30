#ifndef MQTT_IOContextPool_
#define MQTT_IOContextPool_

#include <atomic>
#include <cassert>
#include <thread>
#include <vector>
#include <memory>
#include "asio.hpp"

namespace mqtt {

class IOContextPool
{
public:
  IOContextPool(size_t thread_pool_size);

  void run();

  void stop();

  asio::io_context& get_io_context();

private:
  using io_context_ptr = std::shared_ptr<asio::io_context>;
  using work_ptr = std::shared_ptr<asio::io_context::work>;

  std::vector<io_context_ptr> io_contexts_;
  std::vector<work_ptr> work_;

  std::vector<std::thread> thread_pool_;
  std::atomic<size_t> next_io_ctx_;
};

};

#endif
