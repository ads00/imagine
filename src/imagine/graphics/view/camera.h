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

#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include "imagine/compute/geom/homogeneous.h"
#include "imagine/compute/geom/ray.h"

namespace ig
{

class IG_API camera
{
public:
  enum type_t { orthographic, perspective };

  camera(type_t type, std::size_t w, std::size_t h);
  camera(type_t type, std::size_t w, std::size_t h, const vec3& p, const vec3& t, const vec3& u);

  void update();

  void make_orthographic();
  void make_perspective(float fovy);

  void clip(float zn, float zf);

  ray cast_ray(std::size_t x, std::size_t y) const;

private:
  type_t type_;

  std::size_t w_, h_;
  vec3 pos_, target_, up_;

  float zn_, zf_;
  float fovy_;

  bool uview_, uproj_;
  mat4 view_, proj_,
    iview_, iproj_;
};

} // namespace ig

#endif // GRAPHICS_CAMERA_H
