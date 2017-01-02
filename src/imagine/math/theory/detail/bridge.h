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

#ifndef IG_MATH_BRIDGE_H
#define IG_MATH_BRIDGE_H

#include "imagine/ig.h"

#include <functional>
#include <array>
#include <fstream>

namespace ig  {

template <typename T>
class bridge {
public:
  using gen_t = std::tuple<
    std::function<bool(std::istream&)>, 
    std::function<std::unique_ptr<T>(std::istream&)>, std::function<bool(std::ostream&, const T&)> >;

  static auto load(const std::string& filename) 
  { return table_load(T::bridge_table_, filename); }

  template <typename F>
  static bool save(const std::string& filename, F format, const T& data)
  { return table_save(T::bridge_table_, filename, static_cast<int32_t>(format), data); }

private:
  template <typename table>
  static auto table_load(table& tbl, const std::string& filename) -> std::unique_ptr<T> {
    std::ifstream in{filename, std::ios::binary};
    if (!in.good()) {
      throw std::ios::failure{"(Bridge): Failed to open file --" + filename + "--"};
    }

    for (auto& format_bridge : tbl)
      if (std::get<0>(format_bridge.second)(in)) return std::get<1>(format_bridge.second)(in);
    throw std::runtime_error{"(Bridge): No available entry found in the bridge table"};
  }

  template <typename table>
  static bool table_save(table& tbl, const std::string& filename, int32_t fi, const T& data) {
    std::ofstream out{filename, std::ios::binary | std::ios::trunc};
    return out.good()
      ? std::get<2>(tbl[fi])(out, data)
      : true;
  }
};

enum class ndarray_format : int32_t { 
  unknown = -1, 
  jpeg, png, targa, bmp, pnm, hdr // images
};

} // namespace ig

#endif // IG_MATH_BRIDGE_H
