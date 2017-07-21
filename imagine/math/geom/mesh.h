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

#include "imagine/math/geom/shape.h"

#include <array>
#include <unordered_map>

namespace ig {

template <typename Vertex, template <typename> typename Polygon>
class mesh {
public:
  using f_type = std::array<uint32_t, Polygon<Vertex>::N>;
  using v_type = Vertex;

  mesh() = default;

  auto get_faces_count() const { return std::tuple_size<f_type>::value * faces.size(); }

  auto& get_edges() const    { return edges_; }
  auto& get_topology() const { return topology_; }

  std::vector<f_type> faces;
  std::vector<v_type> vertices;

private:
  struct part {};
  std::vector<part> parts_;

  struct halfedge {
    f_type& face;
    v_type& vertex; halfedge* pair, * next;
  }; std::vector<halfedge> edges_;

  struct topology {
    std::unordered_map<f_type*, halfedge&> F_edge;
    std::unordered_map<v_type*, halfedge&> V_edge;
  } topology_;
};

struct vertex {
  vec3 p;
  vec3 n;
  vec2 uv; };
// utils
struct vertex_pos_color { vec3 p; vec4 c; };

} // namespace ig

#endif // IG_MATH_MESH_H
