/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_BIN_H
#define IG_MATH_BIN_H

#include "imagine/math/geom/proj.h"

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
  template <typename PrimIterator> void pack (const bin_mapping& mapping, PrimIterator begin, PrimIterator end);

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
void bin::pack(const bin_mapping& mapping, PrimIterator begin, PrimIterator end) {
  std::for_each(
    begin,
    end,
    [this, &mapping](auto& primitive) {
      auto binmap = mapping.bin(primitive.bounds.min + primitive.bounds.max);
      for (uint32_t d = 0; d < dimensions; ++d) bounds_[static_cast<size_t>(binmap[d])][d].expand(primitive.bounds);
    });
}

} // namespace ig

#endif // IG_MATH_BIN_H
