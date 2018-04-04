/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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
