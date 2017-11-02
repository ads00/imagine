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

template < typename V >
class mesh {
public:
  using F = std::array<uint32_t, 3>;

  mesh() = default;
  auto operator[](size_t i) const {
    auto& f = faces;
    auto& v = vertices;
    return std::make_unique< triangle<V> >(v[f[i][0]], v[f[i][1]], v[f[i][2]]); }

  std::vector<F> faces;
  std::vector<V> vertices;
};

struct vertex {
  vec3 p;
  vec3 n;
  vec2 uv; };
// utils
struct vertex_pos_color { vec3 p; vec4 c; };

} // namespace ig

#endif // IG_MATH_MESH_H
