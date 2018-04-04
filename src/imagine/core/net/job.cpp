/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/core/net/job.h"

namespace ig {

job::job(size_t workers)
  : running_{true}
  , jobs_{0} {

  for (size_t i = 0; i < workers; ++i)
    workers_.emplace_back([this] {
      for (;;) {
        task task{};
        {
          std::unique_lock<decltype(mutex_)> lock{mutex_};
          cv_.wait(lock, [this] { return !running_ || !tasks_.empty(); });

          if (!running_ && tasks_.empty())
            return;
          task = std::move(tasks_.front());
          tasks_.pop();
        }
        task();
        jobs_--;
        wait_.notify_one();
      }
    });
}

job::~job() {
  running_ = false;
  cv_.notify_all();
  for (auto& worker : workers_) worker.join();
}

void job::wait() {
  std::unique_lock<decltype(mutex_)> lock{mutex_};
  wait_.wait(lock, [this] { return !jobs_; });
}

} // namespace ig
