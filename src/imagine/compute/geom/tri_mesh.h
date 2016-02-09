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

#ifndef COMPUTE_TRI_MESH_H
#define COMPUTE_TRI_MESH_H

#include "imagine/compute/geom/homogeneous.h"

namespace ig
{

class IG_API tri_mesh 
{
public:
  friend struct intersect;
  using face = std::array<std::size_t, 3>;

  tri_mesh() = default;
  constexpr tri_mesh(const std::vector<face>& faces, const std::vector<vec3>& vertices,
                     const std::vector<vec3>& normals = {}, const std::vector<vec2>& coords = {});

  static tri_mesh make_box(unsigned int tess);
  static tri_mesh make_plane(unsigned int tess);
  static tri_mesh make_sphere(unsigned int tess);

private:
  std::vector<face> faces_;

  std::vector<vec3> vertices_, normals_;
  std::vector<vec2> coords_;
};

} // namespace ig

#endif // COMPUTE_TRI_MESH_H