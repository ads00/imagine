/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#ifndef IG_MATH_SHAPE_H
#define IG_MATH_SHAPE_H

#include "imagine/math/geom/proj.h"
#include "imagine/math/geom/spatial/aabb.h"

namespace ig  {

struct interaction {
  float t0, t1, t2;
  float distance = std::numeric_limits<float>::infinity(); };

struct parametric {
  vec3 p;
  vec3 n;
  vec2 uv;
  vec3 dpdu, dpdv; vec3 dndu, dndv; };

class shape {
public:
  virtual float pdf() const 
  { return 1.f / area(); }

  virtual aabb bounding() const = 0;
  virtual float area() const = 0;

  virtual bool occlu(const vec3& o, const vec3& d, float& thit) const = 0;
  virtual bool query(const vec3& o, const vec3& d, float& thit, interaction& i) const = 0;
  virtual bool compute_parametric(const interaction& i, parametric& p) const = 0;
};

} // namespace ig

#endif // IG_MATH_SHAPE_H
