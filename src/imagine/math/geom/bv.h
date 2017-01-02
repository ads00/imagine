/*
 Copyright (c) 2015, 2016
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

#ifndef IG_MATH_BV_H
#define IG_MATH_BV_H

#include "imagine/math/geom/projective.h"

namespace ig {

class aabb;
class convex;

class IG_API aabb {
public:
  aabb();
  explicit aabb(const vec3& min, const vec3& max);
  explicit aabb(const std::vector<vec3>& points);

  void expand(const vec3& point);

  auto size() const         { return max_ - min_; }
  auto volume() const       { return size().prod(); }
  auto surface_area() const {
    auto ext = size();
    return 2.f * (ext[0] * ext[1] + ext[0] * ext[2] + ext[1] * ext[2]); }

  auto centroid() const { return (min_ + max_) * 0.5f; }

private:
  vec3 min_, max_;
};

class bvh {};

namespace bv {

template <typename T, typename U> bool contain(const T& lhs, const U& rhs);
template <typename T, typename U> bool intersect(const T& lhs, const U& rhs);

} // namespace bv
} // namespace ig

#endif // IG_MATH_BV_H
