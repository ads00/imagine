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

#ifndef IG_CORE_TIME_H
#define IG_CORE_TIME_H

#include "imagine/ig.h"

#include <chrono>
#include <vector>

namespace ig {

template <typename Duration> 
class time_job;
template <typename Chrono>
class time {
public:
  time() = default;

  template <typename Duration, typename Fn, typename... Args> auto measure_once(size_t runs, Fn&& fn, Args&&... args) const;
  template <typename Duration, typename Fn, typename... Args> void measure_each(time_job<Duration>& job, Fn&& fn, Args&&... args) const;

  time(const time&) = delete;
  time& operator=(const time&) = delete;
};

template <typename Duration = std::chrono::microseconds>
class time_job {
public:
  explicit time_job(size_t reset = 1)
    : rr{reset}
    , count{0}
    , samples(reset, Duration{0}) {}

  void generate_stats() {
    std::sort(samples.begin(), samples.end());
    auto lo_id = (count + 1) / 4;
    auto hi_id = (count + 1) * 3 / 4;
    auto sum = std::accumulate(samples.begin() + lo_id, samples.begin() + hi_id, Duration{0});

    stats.lo_q = samples[lo_id].count();
    stats.hi_q = samples[hi_id].count();
    stats.median = samples[count / 2].count();
    stats.average = sum.count() / (hi_id - lo_id);
  }

  size_t rr, count;
  std::vector<Duration> samples;
  struct stats { uint64_t lo_q, hi_q, median, average; } stats{};
};

template <typename Chrono>
template <typename Duration, typename Fn, typename... Args>
auto time<Chrono>::measure_once(size_t runs, Fn&& fn, Args&&... args) const {
  time_job<Duration> job{runs};
  for (size_t run = 0; run < runs; ++run) {
    auto begin = Chrono::now();
    std::forward<Fn>(fn)(std::forward<Args>(args)...);
    auto end   = Chrono::now();
    job.samples[run] = std::chrono::duration_cast<Duration>(end - begin);
    job.count++;
  }

  job.generate_stats();
  return job;
}

template <typename Chrono>
template <typename Duration, typename Fn, typename... Args>
void time<Chrono>::measure_each(time_job<Duration>& job, Fn&& fn, Args&&... args) const {
  auto begin = Chrono::now();
  std::forward<Fn>(fn)(std::forward<Args>(args)...);
  auto end   = Chrono::now();
  job.samples[job.count] = std::chrono::duration_cast<Duration>(end - begin);

  if (++job.count == job.rr) {
    job.generate_stats(); job.count = 0; }
}

} // namespace ig

#endif // IG_CORE_TIME_H
