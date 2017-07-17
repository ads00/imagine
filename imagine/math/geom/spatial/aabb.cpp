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

// axis-aligned bounding box
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

void aabb::expand(const vec3& point) {
  for (uint32_t i = 0; i < 3; ++i) min[i] = std::min(min[i], point[i]), max[i] = std::max(max[i], point[i]);
}

bool aabb::intersect(const vec3& origin, const vec3& dir_rcp) {
  auto t1 = (min[0] - origin[0]) * dir_rcp[0];
  auto t2 = (max[0] - origin[0]) * dir_rcp[0];

  auto tmin = std::min(t1, t2);
  auto tmax = std::max(t1, t2);

  for (uint32_t i = 1; i < 3; ++i) {
    t1 = (min[i] - origin[i]) * dir_rcp[i];
    t2 = (max[i] - origin[i]) * dir_rcp[i];

    tmin = std::max(tmin, std::min(t1, t2));
    tmax = std::min(tmax, std::max(t1, t2));
  } return tmax > std::max(tmin, 0.f);
}

aabb union_g(const aabb& lhs, const aabb& rhs) 
{ return aabb{minima(lhs.min, rhs.min), maxima(lhs.max, rhs.max)}; }

aabb inter_g(const aabb& lhs, const aabb& rhs) 
{ return aabb{maxima(lhs.min, rhs.min), minima(lhs.max, rhs.max)}; }

} // namespace ig
