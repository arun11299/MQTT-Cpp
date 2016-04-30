#include "io_context_pool.hpp"

using namespace mqtt;

IOContextPool::IOContextPool(size_t thread_pool_size):
  next_io_ctx_(0)
{
  assert (thread_pool_size != 0);

  io_contexts_.reserve(thread_pool_size);
  work_.reserve(thread_pool_size);

  for (size_t i = 0; i < thread_pool_size; i++) {
    io_context_ptr io_context(new asio::io_context);
    work_ptr work(new asio::io_context::work(*io_context));

    io_contexts_.push_back(io_context);
    work_.push_back(work);
  }
}

void IOContextPool::run()
{
  thread_pool_.reserve(io_contexts_.size());

  for (size_t i = 0; i < io_contexts_.size(); i++) {
    auto& io_srv = io_contexts_[i];
    thread_pool_.push_back(
    	std::thread([&io_srv]() {
	   io_srv->run();
	}));
  }

  for (auto& thr : thread_pool_) thr.join();
}

void IOContextPool::stop()
{
  for (int i = 0; i < io_contexts_.size(); i++) {
    io_contexts_[i]->stop();
  }
}

asio::io_context& IOContextPool::get_io_context()
{
  return *io_contexts_[next_io_ctx_++ % io_contexts_.size()];
}
