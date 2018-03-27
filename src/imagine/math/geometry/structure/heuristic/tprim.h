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

  auto size() const
  { return end - begin; }

  void expand(const bbox& bounds);
  void merge (const build_record& o);
  auto compare_open(size_t id, const build_record& o) const;

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
