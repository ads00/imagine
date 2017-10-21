/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
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
