/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_HEURISTIC_BINNER_H
#define IG_MATH_HEURISTIC_BINNER_H

#include "imagine/math/geometry/structure/bin.h"
#include "imagine/math/geometry/structure/heuristic/tprim.h"

namespace ig        {
namespace heuristic {

class binner {
public:
  using cut = typename bin::cut;

  explicit binner(std::vector<build_prim>& primitives, uint32_t splits)
    : primitives_(primitives)
    , splits_{splits}
    , weight_{1.f} {}

  auto find(const build_record& record);
  auto partition(const cut& cut, size_t begin, size_t end);

private:
  std::vector<build_prim>& primitives_;
  uint32_t splits_;
  float weight_;
};

auto binner::find(const build_record& record) {
  bin arr{splits_};
  bin_mapping map{splits_, record.centroid_bounds};

  arr.pack(
    map,
    std::next(
      primitives_.begin(),
      record.begin),
    std::next(
      primitives_.begin(),
      record.end));
  return arr.split(
    map,
    [this](auto& bounds) { return bounds.half_area() * weight_; });
}

auto binner::partition(const cut& cut, size_t begin, size_t end) {
  auto center = (begin + end) / 2;
  if (cut.dimension != bin::invalid) {
    auto part =
      std::partition(
        std::next(primitives_.begin(), begin),
        std::next(primitives_.begin(), end),
        [&cut](auto& primitive) { return cut.mapping.bin(primitive.bounds, cut.index, cut.dimension); });
    center =
      std::distance(
        primitives_.begin(),
        part);
  }

  build_record
  local_left{begin, begin}, local_right{center, center};

  for (auto i = begin;  i < center; ++i) local_left .expand(primitives_[i].bounds);
  for (auto i = center; i < end;    ++i) local_right.expand(primitives_[i].bounds);
  return std::make_pair(
    local_left,
    local_right);
}

} // namespace heuristic
} // namespace ig

#endif // IG_MATH_HEURISTIC_BINNER_H
