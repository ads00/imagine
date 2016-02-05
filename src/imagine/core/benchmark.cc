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

#include "imagine/core/benchmark.h"
#include <vector>

namespace ig
{

void benchmark::run(std::function<void ()> func, const std::string& name, std::size_t runs)
{
  std::vector<std::chrono::microseconds> runner(runs);
  for (std::size_t run = 0; run < runs; ++run)
  {
    const auto chrono_run = std::chrono::high_resolution_clock::now();
    func();
    runner[run] = std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::high_resolution_clock::now() - chrono_run);
  }

  const auto it = benchs_.find(name);
  if (it == benchs_.end())
    benchs_.emplace(name, runner);
  else
  {
    auto& v = it->second;
    if (v.size() < limit_) v.insert(v.end(), runner.begin(), runner.end());
  }
}

std::unordered_map< std::string, std::vector<std::chrono::microseconds> > benchmark::benchs_;
std::size_t benchmark::limit_ = 100000;

} // namespace ig
