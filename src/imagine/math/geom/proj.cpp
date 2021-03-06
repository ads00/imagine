/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/math/geom/proj.h"

namespace ig  {
namespace trf {

mat4 scale(const vec3& s) {
  return mat4{s[0],  0.f,   0.f,   0.f,
              0.f,   s[1],  0.f,   0.f,
              0.f,   0.f,   s[2],  0.f,
              0.f,   0.f,   0.f,   1.f};
}

mat4 rotation(const quat& r) {
  auto& v = r.vector;
  auto
    x = v[0] + v[0],
    y = v[1] + v[1],
    z = v[2] + v[2];

  auto xx = x * v[0], xy = y * v[0], xz = z * v[0];
  auto yy = y * v[1], yz = z * v[1], zz = z * v[2];
  auto wx = x * r.scalar, wy = y * r.scalar, wz = z * r.scalar;

  return mat4{1.f - (yy + zz),  xy - wz,          xz + wy,          0.f,
              xy + wz,          1.f - (xx + zz),  yz - wx,          0.f,
              xz - wy,          yz + wx,          1.f - (xx + yy),  0.f,
              0.f,              0.f,              0.f,              1.f};
}

mat4 translation(const vec3& t) {
  return mat4{1.f,  0.f,  0.f,  t[0],
              0.f,  1.f,  0.f,  t[1],
              0.f,  0.f,  1.f,  t[2],
              0.f,  0.f,  0.f,  1.f};
}

mat4 look(const vec3& eye, const vec3& focus, const vec3& up) {
  auto f = lin::normalise(focus - eye);
  auto s = lin::normalise(lin::cross(up, f));
  auto u = lin::cross(f, s);

  return mat4{s[0],  s[1],  s[2],  lin::dot(s, -eye),
              u[0],  u[1],  u[2],  lin::dot(u, -eye),
              f[0],  f[1],  f[2],  lin::dot(f, -eye),
              0.f,   0.f,   0.f,   1.f};
}

mat4 perspective(float fovy, float asp, float zn, float zf) {
  auto tanh = std::tan(fovy * 0.5f);
  auto ta = asp * tanh;
  auto zr = zf - zn;

  return mat4{1.f / ta,  0.f,         0.f,       0.f,
              0.f,       1.f / tanh,  0.f,       0.f,
              0.f,       0.f,         zf / zr,  -(zf * zn) / zr,
              0.f,       0.f,         1.f ,      0.f};
}

mat4 orthographic(float l, float r, float b, float t, float zn, float zf) {
  auto zr = zf - zn;
  auto ws = r - l, hs = t - b;

  return mat4{2.f / ws,  0.f,       0.f,       -(r + l) / ws,
              0.f,       2.f / hs,  0.f,       -(t + b) / hs,
              0.f,       0.f,       1.f / zr,  -zn / zr,
              0.f,       0.f,       0.f,        1.f};
}

vec3 transform(const mat4& m, const vec3& v, bool unit) {
  auto xp = m(0, 0) * v[0] + m(0, 1) * v[1] + m(0, 2) * v[2];
  auto yp = m(1, 0) * v[0] + m(1, 1) * v[1] + m(1, 2) * v[2];
  auto zp = m(2, 0) * v[0] + m(2, 1) * v[1] + m(2, 2) * v[2];

  if (!unit) {
    return vec3{xp + m(0, 3),
                yp + m(1, 3),
                zp + m(2, 3)} /
                (v[0] * m(3, 0) + v[1] * m(3, 1) + v[2] * m(3, 2) + m(3, 3));
  } else {
    return vec3{xp, yp, zp};
  }
}

} // namespace trf

template <>
template <>
inline auto mat4::eye<true, float>() {
  return mat4{1.f, 0.f, 0.f, 0.f,
              0.f, 1.f, 0.f, 0.f,
              0.f, 0.f, 1.f, 0.f,
              0.f, 0.f, 0.f, 1.f};
}

} // namespace ig
