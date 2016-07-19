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

#ifndef IG_MATH_TRIMESH_H
#define IG_MATH_TRIMESH_H

#include "imagine/math/geom/homogeneous.h"

namespace ig {

class IG_API trimesh {
public:
  using face = std::array<size_t, 3>;

  trimesh() = default;
  trimesh(const std::vector<face>& faces, const std::vector<vec3>& vertices,
          const std::vector<vec3>& normals = {}, const std::vector<vec2>& coords = {});

  static trimesh generate_box(size_t tess);
  static trimesh generate_plane(size_t tess);
  static trimesh generate_sphere(size_t tess);

private:
  std::vector<face> faces_;

  std::vector<vec3> vertices_, normals_;
  std::vector<vec2> coords_;
};

} // namespace ig

#endif // IG_MATH_TRIMESH_H
