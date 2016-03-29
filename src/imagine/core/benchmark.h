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
#include <functional>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace ig
{

struct IG_API benchmark_report
{
  constexpr benchmark_report(const std::string& n, const std::vector<uint64_t>& s);

  std::string name;
  std::vector<uint64_t> samples;
};

class IG_API benchmark
{
public:
  constexpr benchmark() = delete;
  static void run(std::function<void ()> func, const std::string& name, std::size_t runs = 1);

  template <typename T = std::chrono::microseconds>
  static auto reports()
  {
    std::vector<benchmark_report> rep; rep.reserve(benchs_.size());
    for (auto& bench : benchs_)
    {
      std::vector<uint64_t> samples; samples.reserve(bench.second.size());
      std::for_each(bench.second.begin(), bench.second.end(), [&samples](auto& s)
      {
        samples.emplace_back(
          std::chrono::duration_cast<T>(s).count());
      });

      rep.emplace_back(bench.first, samples);
    }

    return rep;
  }

  benchmark(const benchmark&) = delete;
  benchmark& operator=(const benchmark&) = delete;

private:
  static std::unordered_map< std::string, std::vector<std::chrono::microseconds> > benchs_;
};

} // namespace ig

#endif // CORE_BENCHMARK_H
