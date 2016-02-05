/*
 Copyright (c) 2015, 2016
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

#ifndef CORE_THREADPOOL_H
#define CORE_THREADPOOL_H

#include "imagine.h"
#include <future>
#include <queue>

namespace ig
{

class IG_API thread_pool 
{
public:
  thread_pool(std::size_t workers = std::thread::hardware_concurrency());
  ~thread_pool();

  template<class TFn, class... TArgs>
  auto work(TFn&& f, TArgs&&... args)
  {
    using return_t = decltype(f(args...));
    auto task = std::make_shared<
      std::packaged_task<return_t()> >
      (std::bind(std::forward<TFn>(f), std::forward<TArgs>(args)...));

    std::future<return_t> ret = task->get_future();
    {
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      tasks_.emplace([task]() { (*task)(); });
    }

    cv_.notify_one();
    return ret;
  }
  
private:
  std::vector<std::thread> workers_;
  std::queue< std::function<void ()> > tasks_;

  std::mutex mutex_;
  std::condition_variable cv_;

  std::atomic_bool running_;
};

} // namespace ig

#endif // CORE_THREADPOOL_H
