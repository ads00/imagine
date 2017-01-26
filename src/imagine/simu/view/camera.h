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

#ifndef IG_SIMU_CAMERA_H
#define IG_SIMU_CAMERA_H

#include "imagine/math/geom/projective.h"

namespace ig {

class IG_API camera {
public:
  explicit camera(planar_proj planar, size_t w, size_t h);
  explicit camera(planar_proj planar, size_t w, size_t h, const vec3& pos, const vec3& target, const vec3& up);

  void extent(float fovy);
  void clip(float zn, float zf);

  ray3 cast_ray(size_t x, size_t y) const;

  const mat4& view();
  const mat4& proj();

private:
  planar_proj projection_;

  size_t w_, h_;
  vec3 pos_, target_, up_;

  float zn_, zf_;
  float fovy_;

  bool uview_, uproj_;
  mat4 view_, proj_,
    iview_, iproj_;
};

} // namespace ig

#endif // IG_SIMU_CAMERA_H
