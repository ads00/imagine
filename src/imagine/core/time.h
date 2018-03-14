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
#include <numeric>
#include <vector>

namespace ig {

template
< typename Clock,
  typename Duration = std::chrono::milliseconds >
class time {
public:
  explicit time(size_t reset = 1)
    : rr{reset}
    , count{0}
    , samples(reset, Duration{0}) {}

  void generate_stats();
  template <typename Callable, typename... Args> void measure(size_t runs, Callable&& fn, Args&&... args);

  size_t rr, count;
  std::vector<Duration> samples;
  struct stats { uint64_t lo_q, hi_q, median, average; } stats{};
};

template
< typename Clock,
  typename Duration >
void time<Clock, Duration>::generate_stats() {
  std::sort(samples.begin(), samples.end());
  auto lo_id = (count + 1) / 4;
  auto hi_id = (count + 1) * 3 / 4;

  stats.lo_q = samples[lo_id].count();
  stats.hi_q = samples[hi_id].count();
  stats.median = samples[count / 2].count();
  stats.average =
    std::accumulate(
      samples.begin() + lo_id,
      samples.begin() + hi_id,
      Duration{0})
    .count()
    /
    (hi_id - lo_id);
}

template
< typename Clock,
  typename Duration >
template <typename Callable, typename... Args>
void time<Clock, Duration>::measure(size_t runs, Callable&& fn, Args&&... args) {
  for (size_t i = 0; i < runs; ++i) {
    auto begin = Clock::now();
    std::forward<Callable>(fn)(std::forward<Args>(args)...);
    auto end = Clock::now();
    samples[count++] = std::chrono::duration_cast<Duration>(end - begin);
  }

  if (count == rr) {
    generate_stats();
    count = 0;
  }
}

} // namespace ig

#endif // IG_CORE_TIME_H
