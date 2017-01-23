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

namespace ig {

vec3 projective::transform(const vec3& v, bool unit) const {
  auto& s = *this;
  auto xp = s(0, 0) * v[0] + s(0, 1) * v[1] + s(0, 2) * v[2];
  auto yp = s(1, 0) * v[0] + s(1, 1) * v[1] + s(1, 2) * v[2];
  auto zp = s(2, 0) * v[0] + s(2, 1) * v[1] + s(2, 2) * v[2];

  if (!unit) {
    auto us = vec3{xp + s(0, 3), yp + s(1, 3), zp + s(2, 3)};
    auto wp = s(3, 0) * v[0] + s(3, 1) * v[1] + s(3, 2) * v[2] + s(3, 3);
    return us / wp;
  } else {
    return vec3{xp, yp, zp};
  }
}

projective projective::translating(const vec3& t) {
  return projective{1.f,  0.f,  0.f,  0.f,
                    0.f,  1.f,  0.f,  0.f,
                    0.f,  0.f,  1.f,  0.f,
                    t[0], t[1], t[2], 1.f};
}

projective projective::rotating(const quat& r) {
  auto& v = r.vec_;
  auto
    x = v[0] + v[0],
    y = v[1] + v[1],
    z = v[2] + v[2];

  auto xx = x * v[0], xy = y * v[0], xz = z * v[0];
  auto yy = y * v[1], yz = z * v[1], zz = z * v[2];
  auto wx = x * r.sca_, wy = y * r.sca_, wz = z * r.sca_;

  return projective{1.f - (yy + zz), xy + wz,         xz - wy,         0.f,
                    xy - wz,         1.f - (xx + zz), yz + wx,         0.f,
                    xz + wy,         yz - wx,         1.f - (xx + yy), 0.f,
                    0.f,             0.f,             0.f,             1.f};
}

projective projective::scaling(const vec3& s) {
  return projective{s[0], 0.f,  0.f,  0.f,
                    0.f,  s[1], 0.f,  0.f,
                    0.f,  0.f,  s[2], 0.f,
                    0.f,  0.f,  0.f,  1.f};
}

projective projective::orthographic(size_t w, size_t h, float zn, float zf) {
  auto zr = zf - zn;
  return projective{1.f / w, 0.f,      0.f,      0.f,
                    0.f,     1.f / h,  0.f,      0.f,
                    0.f,     0.f,     -1.f / zr, 0.f,
                    0.f,     0.f,     -zn / zr,  1.f};
}

projective projective::perspective(float fovy, float asp, float zn, float zf) {
  auto tanh = std::tan(fovy * 0.5f);
  auto ta = asp * tanh;
  auto zr = zf - zn;

  return projective{1.f / ta, 0.f,         0.f,             0.f,
                    0.f,      1.f / tanh,  0.f,             0.f,
                    0.f,      0.f,        -zf / zr,        -1.f,
                    0.f,      0.f,        -(zf * zn) / zr,  0.f};
}

projective projective::look(const vec3& eye, const vec3& focus, const vec3& up) {
  auto R2 = linalg::normalise(eye - focus);
  auto R0 = linalg::normalise(linalg::cross(up, R2));
  auto R1 = linalg::cross(R2, R0);
  auto n = -eye;

  using linalg::dot;
  return projective{R0[0],      R1[0],      R2[0],      0.f,
                    R0[1],      R1[1],      R2[1],      0.f,
                    R0[2],      R1[2],      R2[2],      0.f,
                    dot(R0, n), dot(R1, n), dot(R2, n), 1.f};
}

const projective
  projective::eye = projective{1.f, 0.f, 0.f, 0.f,
                               0.f, 1.f, 0.f, 0.f,
                               0.f, 0.f, 1.f, 0.f,
                               0.f, 0.f, 0.f, 1.f};

} // namespace ig
