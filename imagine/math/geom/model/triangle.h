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

#ifndef IG_MATH_TRIANGLE_H
#define IG_MATH_TRIANGLE_H

#include "imagine/math/geom/shape.h"

namespace ig {

template <typename Vertex>
class triangle : public shape {
public:
  static constexpr size_t N = 3;
  explicit triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
    : v0{v0}
    , v1{v1}
    , v2{v2} {}

  aabb bounding() const override;
  float area() const override;

  bool occlu(const vec3& o, const vec3& d, float& thit) const override;
  bool query(const vec3& o, const vec3& d, float& thit, interaction& i) const override;
  bool compute_parametric(const interaction& i, parametric& p) const override;

protected:
  bool moller_trumbore(const vec3& o, const vec3& d, float& t, float& u, float& v) const;

  Vertex v0, v1, v2;
};

template <typename Vertex>
aabb triangle<Vertex>::bounding() const {
  return aabb{{v0.p,
               v1.p,
               v2.p}};
}

template <typename Vertex>
float triangle<Vertex>::area() const {
  auto dp1 = v1.p - v0.p;
  auto dp2 = v2.p - v0.p;
  return 0.5f * lin::norm(lin::cross(dp1, dp2));
}

template <typename Vertex>
bool triangle<Vertex>::occlu(const vec3& o, const vec3& d, float& thit) const {
  float h, u, v;
  return moller_trumbore(o, d, h, u, v) && h < thit;
}

template <typename Vertex>
bool triangle<Vertex>::query(const vec3& o, const vec3& d, float& thit, interaction& i) const {
  float h, u, v;
  if (moller_trumbore(o, d, h, u, v) && h < thit) {
    i.t0 = 1 - u - v;
    i.t1 = u;
    i.t2 = v;
    i.distance = thit = h;
    return true;
  } return false;
}

template <typename Vertex>
bool triangle<Vertex>::compute_parametric(const interaction& i, parametric& p) const {
  auto bary = [this, &i](auto& x0, auto& x1, auto& x2) { return i.t0 * x0 + i.t1 * x1 + i.t2 * x2; };

  p.p  = bary(v0.p,  v1.p,  v2.p); p.n = lin::normalise(bary(v0.n,  v1.n,  v2.n));
  p.uv = bary(v0.uv, v1.uv, v2.uv);

  auto partials = [this](auto& dx1, auto& dx2, auto& dy1, auto& dy2) {
    auto d = dy1[0] * dy2[1] - dy1[1] * dy2[0];
    auto r = 1.f / d;
    return std::make_pair(
      ( dy2[1] * dx1 - dy1[1] * dx2) * r,
      (-dy2[0] * dx1 + dy1[0] * dx2) * r);
  };

  auto dp1 = v1.p - v0.p;
  auto dp2 = v2.p - v0.p;
  auto dn1 = v1.n - v0.n;
  auto dn2 = v2.n - v0.n;
  auto duv1 = v1.uv - v0.uv;
  auto duv2 = v2.uv - v0.uv;

  std::tie(p.dpdu, p.dpdv) = partials(dp1, dp2, duv1, duv2);
  std::tie(p.dndu, p.dndv) = partials(dn1, dn2, duv1, duv2);
  return true;
}

template <typename Vertex>
bool triangle<Vertex>::moller_trumbore(const vec3& o, const vec3& d, float& h, float& u, float& v) const {
  auto dp1 = v1.p - v0.p;
  auto dp2 = v2.p - v0.p;
  auto p = lin::cross(d, dp2);
  auto x = lin::dot  (dp1, p);
  if (std::abs(x) < std::numeric_limits<float>::epsilon()) return false;

  auto r = 1.f / x;
  auto t = o - v0.p; u = lin::dot(t, p) * r;
  if (u < 0 || u > 1) return false;

  auto q = lin::cross(t, dp1); v = lin::dot(d, q) * r;
  if (v < 0 || u + v > 1) return false;

  h = lin::dot(dp2, q) * r;
  return h > std::numeric_limits<float>::epsilon();
}

} // namespace ig

#endif // IG_MATH_TRIANGLE_H
