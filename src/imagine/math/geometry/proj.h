/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_PROJ_H
#define IG_MATH_PROJ_H

#include "imagine/math/linalg/analysis.h"
#include "imagine/math/geometry/spatial/aabb.h"
#include "imagine/math/geometry/spatial/quaternion.h"

#include "imagine/math/theory/simd_accel/intrinsics.h"

namespace ig {

using mat4 = matrix<float, 4>;
using vec4 = colvec<float, 4>; using vec3 = colvec<float, 3>; using vec2 = colvec<float, 2>;
using quat = quaternion<float>;
using bbox = aabb<float>;

using pmat4 = matrix<packet, 4>;
using pvec4 = colvec<packet, 4>; using pvec3 = colvec<packet, 3>;

enum class planar_proj { perspective, orthographic };
enum class coordinate  { local, world };

namespace trf {

IG_API mat4 scale(const vec3& s);
IG_API mat4 rotation(const quat& r);
IG_API mat4 translation(const vec3& t);

IG_API mat4 look(
  const vec3& eye,
  const vec3& focus,
  const vec3& up);
IG_API mat4 perspective(
  float fovy,
  float asp,
  float zn,
  float zf);
IG_API mat4 orthographic(
  float l, float r,
  float t, float b,
  float zn,
  float zf);

IG_API vec3 transform(
  const mat4& m,
  const vec3& v,
  bool unit = false);

} // namespace trf
} // namespace ig

#endif // IG_MATH_PROJ_H
