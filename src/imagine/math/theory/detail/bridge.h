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
#include <fstream>
#include <unordered_map>

namespace ig     {
namespace detail {

template <typename T>
using gen_t = std::tuple<
  std::function<bool(std::istream&)>, // validate<0> / readp<1> / write<2>
  std::function<std::unique_ptr<T>(std::istream&)>, std::function<bool(std::ostream&, const T&)> >;

template <typename table>
auto table_load(const table& table, const std::string& filename) {
  std::ifstream in{filename, std::ios::binary};
  if (!in.good())
    throw std::ios::failure{"(Bridge): Failed to open file --" + filename + "--"};

  for (auto& format_bridge : table)
    if (std::get<0>(format_bridge.second)(in)) 
      return std::get<1>(format_bridge.second)(in);
  throw std::runtime_error
    {"(Bridge): No available entry found in the bridge table"};
}

template <typename T, typename F, typename table>
bool table_save(const table& table, const std::string& filename, F format, const T& data) {
  std::ofstream out{filename, std::ios::binary | std::ios::trunc};
  return out.good()
    ? std::get<2>(T::bridge_table_[format])(out, data)
    : true;
}

} // namespace detail

template <typename T, typename F>
struct bridge {
  static auto load(const std::string& filename)                          { return detail::table_load(table, filename); }
  static bool save(const std::string& filename, F format, const T& data) { return detail::table_save(table, filename, format, data); }
  static std::unordered_map< F, typename detail::gen_t<T> >
    table; };

} // namespace ig

#endif // IG_MATH_BRIDGE_H
