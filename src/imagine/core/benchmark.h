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

#ifndef CORE_BENCHMARK_H
#define CORE_BENCHMARK_H

#include "imagine.h"

#include <chrono>
#include <unordered_map>
#include <functional>

namespace ig
{

class IG_API benchmark
{
public:
  template <typename T> struct single_report;

  static void run(std::function<void ()> func, const std::string& name, std::size_t runs = 1);
  static void limit(std::size_t limit) { limit_ = limit; }

  template <typename T = std::chrono::microseconds> 
  static auto report()
  {
    std::vector< single_report<T> > reporter;
    for (auto& bench : benchs_)
    {
      const uint64_t total = 
        std::accumulate(bench.second.begin(), bench.second.end(), uint64_t(0),
                        [](const uint64_t& t, const auto& b) { return t + std::chrono::duration_cast<T>(b).count(); });

      const auto minmax = std::minmax_element(bench.second.begin(), bench.second.end());
      const auto min = std::chrono::duration_cast<T>(*(minmax.first));
      const auto max = std::chrono::duration_cast<T>(*(minmax.second));
      const auto avg = static_cast<double>(total / bench.second.size());
      
      reporter.emplace_back(bench.first, total, min.count(), max.count(),
                            avg, bench.second.size());
    }

    return reporter;
  }

  benchmark(const benchmark&) = delete;
  benchmark& operator=(const benchmark&) = delete;

private:
  template <typename T>
  struct single_report
  {
    single_report(const std::string& n, uint64_t t, uint64_t mi, uint64_t ma, double a, std::size_t r)
      : name{n}, total{t}, min{mi}, max{ma}, avg{a}, runs{r} {}
    
    std::string name;
    uint64_t total, min, max;
    double avg; std::size_t runs;
  };

  constexpr benchmark() = default;

  static std::unordered_map< std::string, std::vector<std::chrono::microseconds> > benchs_;
  static std::size_t limit_;
};

} // namespace ig

#endif // CORE_BENCHMARK_H
