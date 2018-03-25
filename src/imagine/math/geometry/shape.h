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

#include "imagine/math/geometry/manifold.h"

namespace ig  {

struct vertex;
// generic vertex for [triangles, curves and patches]

class shape {
public:
  virtual ~shape() = default;
  virtual float pdf() const { return 1.f / area(); }

  virtual bbox bounds() const = 0;
  virtual float area() const = 0;

  virtual bool sample(float x, float y) const = 0;
  virtual bool solve(ray& ray) const = 0;
  virtual bool compute_dg(
    float u,
    float v,
    manifold& dg) const = 0;
};

} // namespace ig

#endif // IG_MATH_SHAPE_H
