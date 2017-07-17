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

#ifndef IG_MATH_BRIDGE_H
#define IG_MATH_BRIDGE_H

#include "imagine/ig.h"

#include <fstream>
#include <vector>

namespace ig     {
namespace detail {

template <typename table>
auto table_load(const table& tbl, const std::string& filename) {
  std::ifstream in{filename, std::ios::binary};
  
  for (auto& format_bridge : tbl) {
    auto& validate = std::get<0>(format_bridge);
    if (!validate) continue;

    auto data_ptr = validate(in)
      ? std::get<1>(format_bridge)(in)
      : nullptr;
    if (data_ptr) return data_ptr;
  }
  throw std::runtime_error
    {"(Bridge): No available entry found in the bridge table"};
}

template <typename T, typename F, typename table>
bool table_save(const table& tbl, const std::string& filename, F format, const T& data) {
  std::ofstream out{filename, std::ios::binary | std::ios::trunc};

  auto& write = std::get<2>(tbl[static_cast<size_t>(format)]);
  return out.good()
    ? write(out, data)
    : false;
}

} // namespace detail

template <typename T, typename F>
struct bridge {
  using type = T;
  using rptr = std::unique_ptr<T>;
  using validate = bool (*)(std::istream&);
  using readp    = rptr (*)(std::istream&);
  using write    = bool (*)(std::ostream&, const T&);
  
  static auto load(const std::string& filename)                          { return detail::table_load(tbl(), filename); }
  static bool save(const std::string& filename, F format, const T& data) { return detail::table_save(tbl(), filename, format, data); }

  using loader = std::tuple<validate, readp, write>;
  using table  = std::vector<loader>;
  static ig_api table& tbl(); 
};

} // namespace ig

#endif // IG_MATH_BRIDGE_H
