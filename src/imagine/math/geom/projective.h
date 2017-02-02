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

#ifndef IG_MATH_PROJECTIVE_H
#define IG_MATH_PROJECTIVE_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/analysis.h"
#include "imagine/math/linalg/spatial/quaternion.h"
#include "imagine/math/linalg/spatial/ray.h"

namespace ig {

using mat4 = matrix<float, 4>;
using vec4 = colvec<float, 4>; using vec3 = colvec<float, 3>; using vec2 = colvec<float, 2>;
using quat = quaternion<float>;

using ray3 = ray<float, 3>;

enum class planar_proj { perspective, orthographic };
enum class coordinate  { local, world };

namespace trf {

mat4 translation(const vec3& t);
mat4 rotation(const quat& r);
mat4 scale(const vec3& s);

mat4 look(const vec3& eye, const vec3& focus, const vec3& up);
mat4 perspective(float fovy, float asp, float zn, float zf);
mat4 orthographic(float l, float r, float t, float b, float zn, float zf);

vec3 transform(const mat4& m, const vec3& v, bool unit = false);

static auto eye() {
  static auto eye = mat4::eye();
  return eye;
}

} // namespace trf
} // namespace ig

#endif // IG_MATH_PROJECTIVE_H
