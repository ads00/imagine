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

#include "imagine/compute/geom/tri_mesh.h"

namespace ig
{

constexpr tri_mesh::tri_mesh(const std::vector<face>& faces, const std::vector<vec3>& vertices,
                             const std::vector<vec3>& normals, const std::vector<vec2>& coords)
  : faces_{faces}, vertices_{vertices}, normals_{normals}, coords_{coords} 
{
}

tri_mesh tri_mesh::make_box(unsigned int tess)
{
  assert(tess > 0 && "Tesselation value must be at least 1");

  const std::size_t nv = 6 * (tess + 1) * (tess + 1);

  std::vector<vec3> v; v.reserve(nv);
  std::vector<vec3> n; n.reserve(nv);
  std::vector<vec2> c; c.reserve(nv);
  std::vector<face> f; f.reserve(12 * tess * tess);

  const auto gen_face = [&tess, &v, &n, &c, &f](const vec3& center, const vec3& uaxis, const vec3& vaxis)
  {
    const std::size_t baseid = v.size();
    const auto norm = linalg::normalise(center);

    for (std::size_t i = 0; i < tess + 1; ++i)
    {
      const float cv = static_cast<float>(i) / tess;
      for (std::size_t j = 0; j < tess + 1; ++j)
      {
        const float cu = static_cast<float>(j) / tess;

        v.emplace_back(center + (cu - 0.5f) * 2.f * uaxis + (cv - 0.5f) * 2.f * vaxis);
        n.emplace_back(norm);
        c.emplace_back(vec2{cu, cv});
      }
    }

    for (std::size_t i = 0; i < tess; ++i)
    {
      for (std::size_t j = 0; j < tess; ++j)
      {
        const std::size_t id = i + j*(tess + 1);
        f.emplace_back(face{baseid + id, baseid + id + tess + 1, baseid + id + 1});
        f.emplace_back(face{baseid + i + 1, baseid + i + tess + 1, baseid + i + tess + 2});
      }
    }
  };

  gen_face({0.5f, 0.f, 0.f}, {0.f, 0.f, 0.5f}, {0.f, 0.5f, 0.f});  // +x
  gen_face({0.f, 0.5f, 0.f}, {0.5f, 0.f, 0.f}, {0.f, 0.f, 0.5f});  // +y
  gen_face({0.f, 0.f, 0.5f}, {0.f, 0.5f, 0.f}, {0.5f, 0.f, 0.f});  // +z
  gen_face({-0.5f, 0.f, 0.f}, {0.f, 0.5f, 0.f}, {0.f, 0.f, 0.5f}); // -x
  gen_face({0.f, -0.5f, 0.f}, {0.f, 0.f, 0.5f}, {0.5f, 0.f, 0.f}); // -y
  gen_face({0.f, 0.f, -0.5f}, {0.5f, 0.f, 0.f}, {0.f, 0.5f, 0.f}); // -z

  return {f, v, n, c};
}

tri_mesh tri_mesh::make_plane(unsigned int tess)
{
  assert(tess > 0 && "Tesselation value must be at least 1");

  const std::size_t nv = (tess + 1) * (tess + 1);

  std::vector<vec3> v; v.reserve(nv);
  std::vector<vec2> c; c.reserve(nv);

  for (std::size_t i = 0; i < tess + 1; ++i)
  {
    const float y = -1.0f + 2.0f*i/tess;
    for (std::size_t j = 0; j < tess + 1; ++j)
    {
      v.emplace_back(vec3{-1.0f + 2.0f*j/tess, y, 0.f});
      c.emplace_back(vec2{static_cast<float>(i) / tess, static_cast<float>(j) / tess});
    }
  }

  std::vector<face> f; f.reserve(2 * tess * tess);
  for (std::size_t i = 0; i < tess; ++i)
  {
    for (std::size_t j = 0; j < tess; ++j)
    {
      const std::size_t id = j + i*(tess + 1);
      f.emplace_back(face{id, id + 1, id + tess + 2});
      f.emplace_back(face{id, id + tess + 2, id + tess + 1});
    }
  }

  return {f, v, {nv, vec3{0.f, 0.f, 1.f}}, c};
}

tri_mesh tri_mesh::make_sphere(unsigned int tess)
{
  assert(tess > 3 && "Tesselation value must be at least 4");

  const std::size_t tess_s = tess + 1;
  const std::size_t tess_r = tess/2 + 1;

  std::vector<vec3> v; v.reserve(tess_s * tess_r);
  std::vector<vec2> c; c.reserve(tess_s * tess_r);

  for (std::size_t r = 0; r < tess_r; ++r)
  {
    const float rh = r * (1.f / (tess_r - 1.f));
    for (std::size_t s = 0; s < tess_s; ++s)
    {
      const float sh = 1.f - s*(1.f / (tess_s - 1.f));

      v.emplace_back(vec3{
        std::sin(two_pi<float>() * sh) * std::sin(pi<float>() * rh),
        std::sin(pi<float>() * (rh - 0.5f)),
        std::cos(two_pi<float>() * sh) * std::sin(pi<float>() * rh)});

      c.emplace_back(vec2{sh, rh});
    }
  }

  std::vector<face> f; f.reserve(2 * (tess_s - 1) * (tess_r - 1));
  for (std::size_t r = 0; r < tess_r - 1; ++r)
  {
    for (std::size_t s = 0; s < tess_s - 1; ++s)
    {
      const std::size_t id = (r + 1)*tess_s;
      f.emplace_back(face{r*tess_s + s + 1, r*tess_s + s, id + s + 1});
      f.emplace_back(face{id + s, id + s + 1, r*tess_s + s});
    }
  }

  return {f, v, v, c};
}

} // namespace ig
