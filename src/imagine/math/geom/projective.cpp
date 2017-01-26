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

#include "imagine/math/geom/projective.h"

namespace ig  {
namespace trf {

mat4 translation(const vec3& t) {
  return mat4{1.f,  0.f,  0.f,  0.f,
              0.f,  1.f,  0.f,  0.f,
              0.f,  0.f,  1.f,  0.f,
              t[0], t[1], t[2], 1.f};
}

mat4 rotation(const quat& r) {
  auto& v = r.vec_;
  auto
    x = v[0] + v[0],
    y = v[1] + v[1],
    z = v[2] + v[2];

  auto xx = x * v[0], xy = y * v[0], xz = z * v[0];
  auto yy = y * v[1], yz = z * v[1], zz = z * v[2];
  auto wx = x * r.sca_, wy = y * r.sca_, wz = z * r.sca_;

  return mat4{1.f - (yy + zz), xy + wz,         xz - wy,         0.f,
              xy - wz,         1.f - (xx + zz), yz + wx,         0.f,
              xz + wy,         yz - wx,         1.f - (xx + yy), 0.f,
              0.f,             0.f,             0.f,             1.f};
}

mat4 scale(const vec3& s) {
  return mat4{s[0], 0.f,  0.f,  0.f,
              0.f,  s[1], 0.f,  0.f,
              0.f,  0.f,  s[2], 0.f,
              0.f,  0.f,  0.f,  1.f};
}

mat4 look(const vec3& eye, const vec3& focus, const vec3& up) {
  auto R2 = linalg::normalise(eye - focus);
  auto R0 = linalg::normalise(linalg::cross(up, R2));
  auto R1 = linalg::cross(R2, R0);
  auto n = -eye;

  using linalg::dot;
  return mat4{R0[0],      R1[0],      R2[0],      0.f,
              R0[1],      R1[1],      R2[1],      0.f,
              R0[2],      R1[2],      R2[2],      0.f,
              dot(R0, n), dot(R1, n), dot(R2, n), 1.f};
}

mat4 orthographic(size_t w, size_t h, float zn, float zf) {
  auto zr = zf - zn;
  return mat4{1.f / w, 0.f,      0.f,      0.f,
              0.f,     1.f / h,  0.f,      0.f,
              0.f,     0.f,     -1.f / zr, 0.f,
              0.f,     0.f,     -zn / zr,  1.f};
}

mat4 perspective(float fovy, float asp, float zn, float zf) {
  auto tanh = std::tan(fovy * 0.5f);
  auto ta = asp * tanh;
  auto zr = zf - zn;

  return mat4{1.f / ta, 0.f,         0.f,             0.f,
              0.f,      1.f / tanh,  0.f,             0.f,
              0.f,      0.f,        -zf / zr,        -1.f,
              0.f,      0.f,        -(zf * zn) / zr,  0.f};
}

vec3 transform(const mat4& m, const vec3& v, bool unit) {
  auto xp = m(0, 0) * v[0] + m(0, 1) * v[1] + m(0, 2) * v[2];
  auto yp = m(1, 0) * v[0] + m(1, 1) * v[1] + m(1, 2) * v[2];
  auto zp = m(2, 0) * v[0] + m(2, 1) * v[1] + m(2, 2) * v[2];

  if (!unit) {
    auto us = vec3{xp + m(0, 3), yp + m(1, 3), zp + m(2, 3)};
    auto wp = m(3, 0) * v[0] + m(3, 1) * v[1] + m(3, 2) * v[2] + m(3, 3);
    return vec3{us / wp};
  } else {
    return vec3{xp, yp, zp};
  }
}

} // namespace trf
} // namespace ig
