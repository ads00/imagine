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

#ifndef IG_MATH_HEURISTIC_BINNED_H
#define IG_MATH_HEURISTIC_BINNED_H

#include "imagine/math/geom/structure/binning.h"
#include "imagine/math/geom/structure/priminfo.h"

namespace ig        {
namespace heuristic {

class binned {
public:
  using binner = bin;
  using cut    = typename binner::cut;

  struct record {
    auto size() const { return end - begin; }
    auto expand(const bbox& bounds) { geometry_bounds.expand(bounds); centroid_bounds.expand(bounds.min + bounds.max); ++end; }
    size_t begin, end;
    bbox geometry_bounds, centroid_bounds; };

  explicit binned(std::vector<prim_info>& primitives, uint32_t splits)
    : primitives_(primitives)
    , splits_{splits} {}

  auto find(const record& record);
  auto partition(const cut& cut, size_t begin, size_t end);

private:
  std::vector<prim_info>& primitives_;
  uint32_t splits_;
};

auto binned::find(const record& record) {
  bin_mapping map{splits_, record.centroid_bounds};
  return bin{splits_}
    .pack(std::next(primitives_.begin(), record.begin), std::next(primitives_.begin(), record.end), map)
    .split(map, [](auto& bounds) { return bounds.half_area(); });
}

auto binned::partition(const cut& cut, size_t begin, size_t end) {
  size_t center = cut.dimension != bin::invalid
    ? std::distance(
        primitives_.begin(),
        std::partition(
          std::next(primitives_.begin(), begin),
          std::next(primitives_.begin(), end),
          [&cut](auto& primitive) { return cut.mapping.bin(primitive.bounds, cut.index, cut.dimension); }))
    : (begin + end) / 2;

  record
  local_left{begin, begin}, local_right{center, center};

  for (auto i = begin;  i < center; ++i) local_left .expand(primitives_[i].bounds);
  for (auto i = center; i < end;    ++i) local_right.expand(primitives_[i].bounds);
  return std::make_pair(
    local_left,
    local_right);
}

} // namespace heuristic
} // namespace ig

#endif // IG_MATH_HEURISTIC_BINNED_H
