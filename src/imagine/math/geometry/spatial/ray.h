/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_RAY_H
#define IG_MATH_RAY_H

#include "imagine/math/geometry/proj.h"

namespace ig {

class ray {
public:
  auto operator()(float d) const
  { return ori + d * dir; }

  vec3 ori;
  vec3 dir;

  float time;
  float tnear, tfar;
  float u, v;
};

} // namespace ig

#endif // IG_MATH_RAY_H
