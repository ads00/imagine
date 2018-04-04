/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_HEURISTIC_TPRIM_H
#define IG_MATH_HEURISTIC_TPRIM_H

#include "imagine/math/geometry/proj.h"

namespace ig {

struct build_prim {
  explicit build_prim(size_t id, const bbox& bounds) : id{id}, bounds{bounds} {}
  size_t id;
  bbox bounds;
};

class build_record {
public:
  build_record() = default;
  explicit build_record(size_t begin, size_t end) : begin{begin}, end{end} {}

  auto size() const { return end - begin; }
  void expand(const bbox& bounds);
  void merge (const build_record& o);

public:
  size_t begin, end;
  bbox geometry_bounds, centroid_bounds;
};

void build_record::expand(const bbox& bounds) {
  geometry_bounds.expand(bounds);
  centroid_bounds.expand(bounds.min + bounds.max);
  end++;
}

void build_record::merge(const build_record& o) {
  geometry_bounds.expand(o.geometry_bounds);
  centroid_bounds.expand(o.centroid_bounds);
  begin += o.begin;
  end   += o.end;
}

} // namespace ig

#endif // IG_MATH_HEURISTIC_TPRIM_H
