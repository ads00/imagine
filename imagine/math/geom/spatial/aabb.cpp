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

#include "imagine/math/geom/spatial/aabb.h"

namespace ig {

aabb::aabb()
  : min{ std::numeric_limits<float>::infinity()}
  , max{-std::numeric_limits<float>::infinity()} {}

aabb::aabb(const vec3& min, const vec3& max)
  : min{min}
  , max{max} {}

aabb::aabb(const std::vector<vec3>& points) : aabb{} {
  for (auto& point : points)
    expand(point);
}

void aabb::expand(const vec3& p) {
  for (uint32_t i = 0; i < 3; ++i) min[i] = std::min(min[i], p[i]), max[i] = std::max(max[i], p[i]);
}

bool aabb::intersect(const vec3& o, const vec3& dr, float& tmin, float& tmax) const {
  float tx1 = (min[0] - o[0]) * dr[0];
  float tx2 = (max[0] - o[0]) * dr[0];
  float ty1 = (min[1] - o[1]) * dr[1];
  float ty2 = (max[1] - o[1]) * dr[1];
  float tz1 = (min[2] - o[2]) * dr[2];
  float tz2 = (max[2] - o[2]) * dr[2];

  tmin = std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)), std::min(tz1, tz2));
  tmax = std::min(std::min(std::max(tx1, tx2), std::max(ty1, ty2)), std::max(tz1, tz2));

  if (tmax < 0)    return false;
  if (tmin > tmax) return false;
  return true;
}

aabb union_g(const aabb& lhs, const aabb& rhs) 
{ return aabb{minima(lhs.min, rhs.min), maxima(lhs.max, rhs.max)}; }

aabb inter_g(const aabb& lhs, const aabb& rhs) 
{ return aabb{maxima(lhs.min, rhs.min), minima(lhs.max, rhs.max)}; }

} // namespace ig
