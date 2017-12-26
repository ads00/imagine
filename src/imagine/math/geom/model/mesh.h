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

#ifndef IG_MATH_MESH_H
#define IG_MATH_MESH_H

#include "imagine/math/geom/model/implicit/triangle.h"

#include <array>

namespace ig {

template <typename Manifold>
class mesh {
public:
  using face = std::array<uint32_t, 3>;
  using edge = std::array<uint32_t, 2>;

  mesh() = default;

  template <typename Vertex = vertex> auto operator[](size_t i) const;

  auto center_of_mass() const;

  std::vector<face>   faces;
  std::vector<vertex> vertices;

protected:
  // topology
};

struct surface_m {};
using mesh_s = mesh<surface_m>;
// vertex
struct vertex {
  vec3 p;
  vec3 n;
  vec2 uv; };
// utils
struct vertex_pos_color { vec3 p; vec4 c; };

template <typename Manifold>
template <typename Vertex>
auto mesh<Manifold>::operator[](size_t i) const {
  using implicit = triangle<Vertex>;

  auto& f = faces;
  auto& v = vertices;
  return std::make_unique<implicit>(v[f[i][0]], v[f[i][1]], v[f[i][2]]);
}

template <typename Manifold>
auto mesh<Manifold>::center_of_mass() const {
  assert(!faces.empty() && "Center of mass is undefined for empty mesh sets");

  vec3 c;
  auto w = 0.f;
  for (auto& face : faces) {
    auto& v0 = vertices[face[0]];
    auto& v1 = vertices[face[1]];
    auto& v2 = vertices[face[2]];

    auto face_centroid = (v0.p + v1.p + v2.p) / 3.f;
    auto weight = lin::norm(0.5f * lin::cross(v1.p - v0.p, v2.p - v0.p));
    c += weight * face_centroid;
    w += weight;
  } return c / w;
}

} // namespace ig

#endif // IG_MATH_MESH_H
