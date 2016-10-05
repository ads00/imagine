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

#include "imagine/math/geom/trimesh.h"

namespace ig {

trimesh::trimesh(const std::vector<face>& faces, const std::vector<vec3>& vertices,
                 const std::vector<vec3>& normals, const std::vector<vec2>& coords)
  : faces_(faces)
  , vertices_(vertices)
  , normals_(normals)
  , coords_(coords) {}

trimesh trimesh::generate_box(size_t tess) {
  assert(tess > 0 && "Tesselation value must be at least 1");
  trimesh box{};
  auto nv = 6 * (tess + 1) * (tess + 1);

  auto gen_face = [&tess, &box](const vec3& center, const vec3& uaxis, const vec3& vaxis) {
    auto baseid = box.vertices_.size();
    auto norm = linalg::normalise(center);

    for (size_t i = 0; i < tess + 1; ++i) {
      auto cv = static_cast<float>(i) / tess;
      for (size_t j = 0; j < tess + 1; ++j) {
        auto cu = static_cast<float>(j) / tess;
        box.vertices_.emplace_back(center + (cu - 0.5f) * 2.f * uaxis + (cv - 0.5f) * 2.f * vaxis);
        box.normals_ .emplace_back(norm);
        box.coords_  .emplace_back(cu, cv);
      }
    }

    for (size_t i = 0; i < tess; ++i) {
      for (size_t j = 0; j < tess; ++j) {
        auto id = i + j * (tess + 1);
        box.faces_.emplace_back(face{baseid + id, baseid + id + tess + 1, baseid + id + 1});
        box.faces_.emplace_back(face{baseid + i + 1, baseid + i + tess + 1, baseid + i + tess + 2});
      }
    }
  };

  box.vertices_.reserve(nv); box.normals_.reserve(nv); box.coords_.reserve(nv);
  box.faces_.reserve(12 * tess * tess);

  gen_face(vec3{0.5f, 0.f, 0.f},  vec3{0.f, 0.f, 0.5f}, vec3{0.f, 0.5f, 0.f}); // +x
  gen_face(vec3{0.f, 0.5f, 0.f},  vec3{0.5f, 0.f, 0.f}, vec3{0.f, 0.f, 0.5f}); // +y
  gen_face(vec3{0.f, 0.f, 0.5f},  vec3{0.f, 0.5f, 0.f}, vec3{0.5f, 0.f, 0.f}); // +z
  gen_face(vec3{-0.5f, 0.f, 0.f}, vec3{0.f, 0.5f, 0.f}, vec3{0.f, 0.f, 0.5f}); // -x
  gen_face(vec3{0.f, -0.5f, 0.f}, vec3{0.f, 0.f, 0.5f}, vec3{0.5f, 0.f, 0.f}); // -y
  gen_face(vec3{0.f, 0.f, -0.5f}, vec3{0.5f, 0.f, 0.f}, vec3{0.f, 0.5f, 0.f}); // -z
  return box;
}

trimesh trimesh::generate_plane(size_t tess) {
  assert(tess > 0 && "Tesselation value must be at least 1");
  trimesh pln{};
  auto nv = (tess + 1) * (tess + 1);

  pln.vertices_.reserve(nv); pln.normals_.resize(nv, vec3{0.f, 0.f, 1.f}); pln.coords_.reserve(nv);
  for (size_t i = 0; i < tess + 1; ++i) {
    auto y = -1.f + 2.f * i / tess;
    for (size_t j = 0; j < tess + 1; ++j) {
      pln.vertices_.emplace_back(-1.f + 2.f * j / tess, y, 0.f);
      pln.coords_  .emplace_back(static_cast<float>(i) / tess, static_cast<float>(j) / tess);
    }
  }

  pln.faces_.reserve(2 * tess * tess);
  for (size_t i = 0; i < tess; ++i) {
    for (size_t j = 0; j < tess; ++j) {
      auto id = j + i * (tess + 1);
      pln.faces_.emplace_back(face{id, id + 1, id + tess + 2});
      pln.faces_.emplace_back(face{id, id + tess + 2, id + tess + 1});
    }
  }
  return pln;
}

trimesh trimesh::generate_sphere(size_t tess) {
  assert(tess > 3 && "Tesselation value must be at least 4");
  trimesh sph{};
  auto tess_s = tess + 1, tess_r = tess / 2 + 1, nv = tess_s * tess_r;

  sph.vertices_.reserve(nv); sph.normals_.reserve(nv); sph.coords_.reserve(nv);
  for (size_t r = 0; r < tess_r; ++r) {
    auto rh = r * (1.f / (tess_r - 1.f));
    for (size_t s = 0; s < tess_s; ++s) {
      auto sh = 1.f - s * (1.f / (tess_s - 1.f));
      auto vn = vec3{
        std::sin(two_pi<float> * sh) * std::sin(pi<float> * rh),
        std::sin(pi<float> * (rh - 0.5f)),
        std::cos(two_pi<float> * sh) * std::sin(pi<float> * rh)};

      sph.vertices_.emplace_back(vn);
      sph.normals_ .emplace_back(vn);
      sph.coords_  .emplace_back(sh, rh);
    }
  }

  sph.faces_.reserve(2 * (tess_s - 1) * (tess_r - 1));
  for (size_t r = 0; r < tess_r - 1; ++r) {
    for (size_t s = 0; s < tess_s - 1; ++s) {
      auto id = (r + 1) * tess_s;
      sph.faces_.emplace_back(face{r * tess_s + s + 1, r * tess_s + s, id + s + 1});
      sph.faces_.emplace_back(face{id + s, id + s + 1, r * tess_s + s});
    }
  }
  return sph;
}

} // namespace ig
