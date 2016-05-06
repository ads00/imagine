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

#include "imagine/nc/geom/homogeneous.h"

namespace ig {

vec3 mat4::transform(const vec3& v, bool unit) const {
  auto&& h = *this;
  auto xp = h(0, 0)*v[0] + h(0, 1)*v[1] + h(0, 2)*v[2];
  auto yp = h(1, 0)*v[0] + h(1, 1)*v[1] + h(1, 2)*v[2];
  auto zp = h(2, 0)*v[0] + h(2, 1)*v[1] + h(2, 2)*v[2];

  if (!unit) {
    auto wp = h(3, 0)*v[0] + h(3, 1)*v[1] + h(3, 2)*v[2] + h(3, 3);
    return vec3{xp + h(0, 3), yp + h(1, 3), zp + h(2, 3)} / wp;
  } return {xp, yp, zp};
}

mat4 mat4::translating(const vec3& t) {
  return { 1.f, 0.f, 0.f, t[0],
           0.f, 1.f, 0.f, t[1],
           0.f, 0.f, 1.f, t[2],
           0.f, 0.f, 0.f, 1.f };
}

mat4 mat4::rotating(const quat& r) {
  auto&& v = r.vec_;
  auto
    x = v[0] + v[0],
    y = v[1] + v[1],
    z = v[2] + v[2];

  auto xx = x * v[0], xy = y * v[0], xz = z * v[0];
  auto yy = y * v[1], yz = z * v[1], zz = z * v[2];
  auto wx = x * r.sca_,  wy = y * r.sca_,  wz = z * r.sca_;

  return { 1.f - (yy + zz), xy - wz,         xz + wy,         0.f,
           xy + wz,         1.f - (xx + zz), yz - wx,         0.f,
           xz - wy,         yz + wx,         1.f - (xx + yy), 0.f,
           0.f,             0.f,             0.f,             1.f };
}

mat4 mat4::scaling(const vec3& s) {
  return { s[0], 0.f,  0.f,  0.f,
           0.f,  s[1], 0.f,  0.f,
           0.f,  0.f,  s[2], 0.f,
           0.f,  0.f,  0.f,  1.f };
}

mat4 mat4::orthographic(std::size_t w, std::size_t h, float zn, float zf) {
  auto zr = zf - zn;
  return { 1.f / w, 0.f,      0.f,       0.f,
           0.f,     1.f / h,  0.f,       0.f,
           0.f,     0.f,     -1.f / zr, -zn / zr,
           0.f,     0.f,      0.f,       1.f };
}

mat4 mat4::perspective(float fovy, float asp, float zn, float zf) {
  auto tanh = std::tan(fovy * 0.5f);
  auto ta = asp * tanh;
  auto zr = zf - zn;

  return { 1.f / ta, 0.f,         0.f,      0.f,
           0.f,      1.f / tanh,  0.f,      0.f,
           0.f,      0.f,         zf / zr, -(zf * zn) / zr,
           0.f,      0.f,        -1.f,      0.f };
}

mat4 mat4::look(const vec3& eye, const vec3& focus, const vec3& up) {
  auto R2 = linalg::normalise(eye - focus);
  auto R0 = linalg::normalise(linalg::cross(up, R2));
  auto R1 = linalg::cross(R2, R0);
  auto neye = -eye;

  return { R0[0], R0[1], R0[2], linalg::dot(R0, neye),
           R1[0], R1[1], R1[2], linalg::dot(R1, neye),
           R2[0], R2[1], R2[2], linalg::dot(R2, neye),
           0.f,   0.f,   0.f,   1.f };
}

const mat4 mat4::eye =
{ 1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  0.f, 0.f, 0.f, 1.f };

} // namespace ig
