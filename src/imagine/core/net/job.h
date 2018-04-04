/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_CORE_JOB_H
#define IG_CORE_JOB_H

#include "imagine/ig.h"

#include <future>
#include <queue>

namespace ig {

class IG_API job {
public:
  explicit job(size_t workers = std::thread::hardware_concurrency());
  ~job();

  void wait();
  template <typename Callable, typename... Args> auto work(Callable&& fn, Args&&... args);

  job(const job&) = delete;
  job& operator=(const job&) = delete;

private:
  std::atomic_bool running_;
  std::atomic_uint jobs_;
  std::mutex mutex_;
  std::condition_variable cv_, wait_;

  std::vector<std::thread> workers_;
  std::queue<task> tasks_;
};

template <typename Callable, typename... Args>
auto job::work(Callable&& fn, Args&&... args) {
  using return_type = std::packaged_task<decltype(fn(args...))()>;
  auto task =
  std::make_shared<return_type>(std::bind(std::forward<Callable>(fn), std::forward<Args>(args)...));

  auto res = task->get_future();
  {
    std::lock_guard<decltype(mutex_)> lock{mutex_};
    tasks_.emplace([task]() { (*task)(); });
  }

  jobs_++;
  cv_.notify_one();
  return res;
}

} // namespace ig

#endif // IG_CORE_JOB_H
