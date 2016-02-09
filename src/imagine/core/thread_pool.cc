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

#include "imagine/core/thread_pool.h"

namespace ig
{

thread_pool::thread_pool(std::size_t workers)
  : running_{true}
{
  const auto thread_work = [this]
  {
    for (;;)
    {
      std::function<void()> task;
      {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        cv_.wait(lock, [this] { return !running_ || !tasks_.empty(); });

        if (!running_ && tasks_.empty())
          return;

        task = std::move(tasks_.front());
        tasks_.pop();
      }

      task();
    }
  };

  for (std::size_t i = 0; i < workers; ++i)
    workers_.emplace_back(thread_work);
}

thread_pool::~thread_pool()
{
  running_ = false;

  cv_.notify_all();
  for (auto& worker : workers_) worker.join();
}

} // namespace ig