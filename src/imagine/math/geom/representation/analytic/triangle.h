/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_TRIANGLE_H
#define IG_MATH_TRIANGLE_H

#include "imagine/math/geom/shape.h"

namespace ig {

template <typename Vertex>
struct triangle : public shape {
public:
  static constexpr size_t N = 3;
  static_assert(std::is_base_of_v<vertex, Vertex>, "Analytic triangle requires base vertex of [position, normal, tangent space]");

  triangle() = default;
  explicit triangle(Vertex v0, Vertex v1, Vertex v2)
    : v0{v0}
    , v1{v1}
    , v2{v2} {}

  virtual bbox bounds() const override;
  virtual float area() const override;

  virtual bool sample(float x, float y) const override;
  virtual bool solve(ray& ray) const override;
  virtual bool compute_dg(
    float u,
    float v,
    manifold& dg) const override;

  Vertex v0, v1, v2;
};

template <typename Vertex>
bbox triangle<Vertex>::bounds() const {
  return bbox{{v0.p,
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
bool triangle<Vertex>::sample(float x, float y) const {
  return true;
}

template <typename Vertex>
bool triangle<Vertex>::solve(ray& ray) const {
  auto e1 = v1.p - v0.p;
  auto e2 = v2.p - v0.p;

  auto p = lin::cross(ray.dir, e2);
  auto x = lin::dot  (e1, p);
  if (std::abs(x) < std::numeric_limits<float>::epsilon()) return false;

  auto r = 1.f / x;
  auto t = ray.ori - v0.p;
  auto u = lin::dot(t, p) * r;
  if (u < 0 || u > 1) return false;

  auto q = lin::cross(t, e1);
  auto v = lin::dot(ray.dir, q) * r;
  if (v < 0 || u + v > 1) return false;

  auto closer = [&](auto d, auto update) { return (ray.tnear < d && ray.tfar > d) ? update(d) : false; };
  return closer(
    lin::dot(e2, q) * r,
    [&](auto& d) { ray.u = u; ray.v = v; ray.tfar = d; return true; });
}

template <typename Vertex>
bool triangle<Vertex>::compute_dg(float u, float v, manifold& dg) const {
  auto bary = [&u, &v](auto bc0, auto bc1, auto bc2) { return (1 - u - v) * bc0 + u * bc1 + v * bc2; };
  auto partials =   [](auto dx1, auto dx2, auto dy1, auto dy2) {
    auto d = dy1[0] * dy2[1] - dy1[1] * dy2[0];
    auto r = 1.f / d;
    return std::make_pair(
      ( dy2[1] * dx1 - dy1[1] * dx2) * r,
      (-dy2[0] * dx1 + dy1[0] * dx2) * r);
  };

  dg.p  = bary(v0.p, v1.p, v2.p);
  dg.n  = lin::normalise(bary(v0.n, v1.n, v2.n));
  dg.uv = bary(v0.uv, v1.uv, v2.uv);
  auto duv1 = v1.uv - v0.uv;
  auto duv2 = v2.uv - v0.uv;
  std::tie(
    dg.dpdu,
    dg.dpdv) = partials(v1.p - v0.p, v2.p - v0.p, duv1, duv2);
  std::tie(
    dg.dndu,
    dg.dndv) = partials(v1.n - v0.n, v2.n - v0.n, duv1, duv2);
  return true;
}

} // namespace ig

#endif // IG_MATH_TRIANGLE_H
