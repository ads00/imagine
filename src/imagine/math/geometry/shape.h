/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_SHAPE_H
#define IG_MATH_SHAPE_H

#include "imagine/math/geometry/topo/manifold.h"

namespace ig  {

struct vertex;
// generic vertex for [triangles, curves and patches]

class shape {
public:
  virtual ~shape() = default;
  virtual float pdf() const { return 1.f / area(); }

  virtual bbox bounds() const = 0;
  virtual float area() const = 0;

  virtual bool sample(float x, float y) const = 0;
  virtual bool solve(ray& ray) const = 0;
  virtual bool compute_dg(
    float u,
    float v,
    manifold& dg) const = 0;
};

} // namespace ig

#endif // IG_MATH_SHAPE_H
