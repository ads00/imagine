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

class projective;
using mat4 
  = projective;

using vec4 = colvec<float, 4>; using vec3 = colvec<float, 3>; using vec2 = colvec<float, 2>;
using quat = quaternion<float>;

using ray3 = ray<float, 3>;

class IG_API projective : public matrix<float, 4> {
public:
  using base_type 
    = matrix<float, 4>;

  projective() = default;

  template <typename... Args>
  explicit projective(T i, Args&&... args)
    : base_type{i, std::forward<Args>(args)...} {}

  template <typename Alg> projective(const alg<Alg>& o) : base_type{o} {}

  vec3 transform(const vec3& v, bool unit = false) const;

  static projective translating(const vec3& t);
  static projective rotating(const quat& r);
  static projective scaling(const vec3& s);
  
  static projective orthographic(size_t w, size_t h, float zn, float zf);
  static projective perspective(float fovy, float asp, float zn, float zf);
  static projective look(const vec3& eye, const vec3& focus, const vec3& up);

  static const projective eye;
};

enum class planar_proj { orthographic, perspective };
enum class coordinate  { local, world };

} // namespace ig

#endif // IG_MATH_PROJECTIVE_H
