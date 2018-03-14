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

#ifndef IG_MATH_BINNING_H
#define IG_MATH_BINNING_H

#include "imagine/math/geometry/proj.h"

namespace ig {

class bin_mapping {
public:
  explicit bin_mapping(uint32_t bins, const bbox& centroid)
    : bins_{bins}
    , bucket_{centroid.min}
    , scale_{(0.99f * bins_) / centroid.extent()} {}

  auto bin(const vec3& point) const { return (point - bucket_) * scale_; }
  auto bin(const bbox& bounds, size_t pivot, int dimension) const { return bin(bounds.min + bounds.max)[dimension] < pivot; }

  auto bins() const { return bins_; }
  auto empty(uint32_t dimension) const { return scale_[dimension] == 0.0f; }

private:
  uint32_t bins_;
  vec3 bucket_, scale_;
};

class bin {
public:
  enum axis {
    invalid = -1,
    x = 0,
    y = 1,
    z = 2, dimensions = 3 };
  using axis_bounds = std::array<bbox, 3>;

  struct cut {
    bin_mapping mapping; axis dimension; size_t index; float cost; };

  explicit bin(uint32_t bins)
    : bounds_(bins) {}

  template <typename Cost>         auto split(const bin_mapping& mapping, Cost&& bounds_cost) const;
  template <typename PrimIterator> auto& pack(PrimIterator begin, PrimIterator end, const bin_mapping& mapping);

private:
  std::vector<axis_bounds> bounds_;
};

template <typename Cost>
auto bin::split(const bin_mapping& mapping, Cost&& bounds_cost) const {
  std::vector<float> left_costs(mapping.bins());

  axis   split = invalid;
  size_t pivot = mapping.bins() / 2;
  float  cost  = std::numeric_limits<float>::max();

  for (uint32_t d = 0; d < dimensions; ++d) {
    bbox acc;
    for (size_t i = 0; i < mapping.bins() - 1; ++i) acc.expand(bounds_[i][d]), left_costs[i] = bounds_cost(acc);

    acc.invalidate();
    for (size_t i = mapping.bins(); i-- > 1; ) {
      acc.expand(bounds_[i][d]);
      auto split_cost = left_costs[i - 1] * i + bounds_cost(acc) * (mapping.bins() - i);

      if (cost > split_cost) {
        split = static_cast<axis>(d);
        pivot = i;
        cost = split_cost;
      }
    }
  }
  return cut{
    mapping,
    static_cast<axis>(split),
    pivot,
    cost};
}

template <typename PrimIterator>
auto& bin::pack(PrimIterator begin, PrimIterator end, const bin_mapping& mapping) {
  std::for_each(
    begin,
    end,
    [this, &mapping](auto& primitive) {
      auto bounds = primitive.bounds;
      auto binmap = mapping.bin(bounds.min + bounds.max);
      for (uint32_t d = 0; d < dimensions; ++d) bounds_[static_cast<size_t>(binmap[d])][d].expand(bounds);
    }); return *this;
}

} // namespace ig

#endif // IG_MATH_BINNING_H
