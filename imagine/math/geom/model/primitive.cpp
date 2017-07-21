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

#include "imagine/math/geom/model/primitive.h"

namespace ig   {
namespace pgen {

tmesh<vertex> box(uint32_t subdiv, const vec3& size) {
  tmesh<vertex> box{};
  box.faces   .reserve(12 * subdiv * subdiv);
  box.vertices.reserve(6 * (subdiv + 1) * (subdiv + 1));

  auto gen_face = [&subdiv, &box](const vec3& center, const vec3& uaxis, const vec3& vaxis) {
    auto normal = lin::normalise(center);
    auto baseid = static_cast<uint32_t>(box.vertices.size());

    for (uint32_t i = 0; i <= subdiv; ++i) {
      auto v = i / static_cast<float>(subdiv);
      for (uint32_t j = 0; j <= subdiv; ++j) {
        auto u = j / static_cast<float>(subdiv);

        box.vertices.emplace_back(vertex {
          vec3{center + (u - 0.5f) * 2.f * uaxis + (v - 0.5f) * 2.f * vaxis},
          vec3{normal},
          vec2{u, v} });
      }
    }

    for (uint32_t i = 0; i < subdiv; ++i) {
      for (uint32_t j = 0; j < subdiv; ++j) {
        auto id = i + j * (subdiv + 1);
        box.faces.emplace_back(tmesh<vertex>::f_type{
          baseid + id,
          baseid + id + subdiv + 1, 
          baseid + id + 1 });
        box.faces.emplace_back(tmesh<vertex>::f_type{
          baseid + id + 1, 
          baseid + id + subdiv + 1, 
          baseid + id + subdiv + 2 });
      }
    }
  };

  gen_face(vec3{size[0], 0.f, 0.f}, vec3{0.f, 0.f, size[2]}, vec3{0.f, size[1], 0.f}); // +x
  gen_face(vec3{0.f, size[1], 0.f}, vec3{size[0], 0.f, 0.f}, vec3{0.f, 0.f, size[2]}); // +y
  gen_face(vec3{0.f, 0.f, size[2]}, vec3{0.f, size[1], 0.f}, vec3{size[0], 0.f, 0.f}); // +z
  gen_face(vec3{-size[0], 0.f, 0.f}, vec3{0.f, size[1], 0.f}, vec3{0.f, 0.f, size[2]}); // -x
  gen_face(vec3{0.f, -size[1], 0.f}, vec3{0.f, 0.f, size[2]}, vec3{size[0], 0.f, 0.f}); // -y
  gen_face(vec3{0.f, 0.f, -size[2]}, vec3{size[0], 0.f, 0.f}, vec3{0.f, size[1], 0.f}); // -z
  return box;
}

tmesh<vertex> plane(uint32_t subdiv, const vec2& size) {
  tmesh<vertex> pln{};
  pln.faces   .reserve(subdiv * subdiv);
  pln.vertices.reserve((subdiv + 1) * (subdiv + 1));

  for (uint32_t i = 0; i < subdiv + 1; ++i) {
    auto u = i * (1.f / subdiv);
    for (uint32_t j = 0; j < subdiv + 1; ++j) {
      auto v = j * (1.f / subdiv);

      pln.vertices.emplace_back(vertex {
        vec3{size[0] * (u - 0.5f), size[1] * (v - 0.5f), 0.f},
        vec3{0.f, 0.f, 1.f},
        vec2{u, v} });
    }
  }

  for (uint32_t i = 0; i < subdiv; ++i) {
    for (uint32_t j = 0; j < subdiv; ++j) {
      auto id = j + i * (subdiv + 1);
      pln.faces.emplace_back(tmesh<vertex>::f_type{
        id,
        id + 1, 
        id + subdiv + 1 });
      pln.faces.emplace_back(tmesh<vertex>::f_type{
        id + subdiv + 1, 
        id + 1, 
        id + subdiv + 2 });
    }
  }
  return pln;
}

tmesh<vertex> uv_sphere(uint32_t subdiv, float radius) {
  tmesh<vertex> sph{};
  auto segments = subdiv + 1;
  auto rings    = segments * 2 - 1;

  sph.faces   .reserve(6 * segments * rings);
  sph.vertices.reserve(segments * rings);

  for (uint32_t r = 0; r < rings; ++r) {
    auto v = r * (1.f / (rings - 1.f));
    for (uint32_t s = 0; s < segments; ++s) {
      auto u = 1.f - s * (1.f / (segments - 1.f));
      auto x = std::sin(two_pi<float> * u) * std::sin(pi<float> * v),
           y = std::sin(pi<float> * (v - 0.5f)),
           z = std::cos(two_pi<float> * u) * std::sin(pi<float> * v);

      sph.vertices.emplace_back(vertex {
        vec3{x * radius, y * radius, z * radius},
        vec3{x, y, z},
        vec2{u, v} });
    }
  }

  for (uint32_t r = 0; r < rings - 1; ++r) {
    for (uint32_t s = 0; s < segments - 1; ++s) {
      auto id = (r + 1) * segments;
      sph.faces.emplace_back(tmesh<vertex>::f_type{
        r * segments + s + 1, 
        r * segments + s, 
        id + s + 1 });
      sph.faces.emplace_back(tmesh<vertex>::f_type{
        id + s,
        id + s + 1,
        r * segments + s });
    }
  }
  return sph;
}

std::vector<vertex_pos_color> frame(float size) {
  std::vector<vertex_pos_color> frm{};

  frm.emplace_back(vertex_pos_color{vec3{0.f, 0.f, 0.f}, vec4{1.f, 0.f, 0.f, 1.f}});
  frm.emplace_back(vertex_pos_color{vec3{1.f, 0.f, 0.f}, vec4{1.f, 0.f, 0.f, 1.f}});
  frm.emplace_back(vertex_pos_color{vec3{0.f, 0.f, 0.f}, vec4{0.f, 1.f, 0.f, 1.f}});
  frm.emplace_back(vertex_pos_color{vec3{0.f, 1.f, 0.f}, vec4{0.f, 1.f, 0.f, 1.f}});
  frm.emplace_back(vertex_pos_color{vec3{0.f, 0.f, 0.f}, vec4{0.f, 0.f, 1.f, 1.f}});
  frm.emplace_back(vertex_pos_color{vec3{0.f, 0.f, 1.f}, vec4{0.f, 0.f, 1.f, 1.f}});
  return frm;
}

} // namespace pgen
} // namespace ig
