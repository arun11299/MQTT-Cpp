#include "IOContextPool.hpp"

using namespace mqtt;

IOContextPool::IOContextPool(size_t thread_pool_size):
  io_contexts_(thread_pool_size),
  work_(thread_pool_size)
{
  assert (thread_pool_size != 0);

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
    threads.push_back(std::thread(
    	  &asio::io_context::run, io_contexts_[i]));
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
