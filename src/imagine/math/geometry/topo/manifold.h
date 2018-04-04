/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MANIFOLD_H
#define IG_MATH_MANIFOLD_H

#include "imagine/math/geometry/proj.h"
#include "imagine/math/geometry/spatial/ray.h"

namespace ig  {

struct manifold {
  vec3 p;
  vec3 n;
  vec2 uv;
  vec3 dpdu, dpdv; vec3 dndu, dndv; };

} // namespace ig

#endif // IG_MATH_MANIFOLD_H
