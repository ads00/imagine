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

#ifndef IG_CORE_BENCHMARK_H
#define IG_CORE_BENCHMARK_H

#include "imagine.h"

#include <chrono>
#include <functional>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace ig {

class benchmark_report;
class benchmark {
public:
  benchmark() = default;

  template <typename Fn, typename... Args>
  void measure(const std::string& name, size_t runs , Fn&& fn, Args&&... args) {
    auto runner = std::vector<std::chrono::microseconds>(runs);
    for (size_t run = 0; run < runs; ++run) {
      auto begin = std::chrono::high_resolution_clock::now();
      fn(std::forward<Args>(args)...);
      runner[run] = std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::high_resolution_clock::now() - begin);
    }

    auto it = benchs_.find(name);
    if (it != benchs_.end()) {
      it->second.insert(it->second.end(), runner.begin(), runner.end());
    } else {
      benchs_.emplace(name, runner);
    }
  }

  template <typename T = std::chrono::microseconds>
  auto generate_reports() {
    auto reports = std::vector<benchmark_report>{benchs_.size()};
    std::transform(benchs_.begin(), benchs_.end(), reports.begin(), [](const auto& bench) {
      auto report = benchmark_report{bench.first};

      std::for_each(bench.second.begin(), bench.second.end(), [&report](const auto& sample) {
        report.samples.emplace_back(std::chrono::duration_cast<T>(sample).count());
      });
      return std::move(report);
    });
    return reports;
  }

  benchmark(const benchmark&) = delete;
  benchmark& operator=(const benchmark&) = delete;

private:
  std::unordered_map< std::string, std::vector<std::chrono::microseconds> > benchs_;
};

class benchmark_report {
public:
  std::string name;
  std::vector<uint64_t> samples;
};

} // namespace ig

#endif // IG_CORE_BENCHMARK_H
