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

#ifndef COMPUTE_QUATERNION_H
#define COMPUTE_QUATERNION_H

#include "imagine/compute/linalg/matrix.h"

namespace ig
{

template <typename T>
class quaternion
{
public:
  using vec_t = vector<T, 3>;

  constexpr quaternion() : s{1}, v{} {}
  constexpr quaternion(T scalar, const vec_t& vec)
    : s{scalar}, v{vec} {}

  quaternion& operator+=(const quaternion& q); 
  quaternion& operator*=(const quaternion& q);

  quaternion& operator/=(T scalar); quaternion& operator*=(T scalar);

  static quaternion axis(T angle, const vec_t& axis)
  {
    return { std::cos(angle / T(2)), axis * std::sin(angle / T(2)) };
  }

  static quaternion euler(T roll, T pitch, T yaw)
  {
    const T cr = std::cos(roll);  const T sr = std::sin(roll);
    const T cp = std::cos(pitch); const T sp = std::sin(pitch);
    const T cy = std::cos(yaw);   const T sy = std::sin(yaw);

    return { cr*cp*cy - sr*sp*sy, { cr*cp*cy + sr*sp*sy,
                                    sr*cp*cy + cr*sp*sy,
                                    cr*sp*cy - sr*cp*sy } };
  }

  T s;
  vec_t v;
};

template <typename T>
auto quaternion<T>::operator+=(const quaternion& q) -> quaternion&
{
  s += q.s; v += q.v;
  return *this;
}

template <typename T>
auto quaternion<T>::operator*=(const quaternion& q) -> quaternion&
{
  const auto p = *this;

  s = p.s*q.s - linalg::dot(p.v, q.v);
  v = p.s*q.v + q.s*p.v + linalg::cross(p.v, q.v);
  return *this;
}

template <typename T>
auto quaternion<T>::operator/=(T scalar) -> quaternion&
{
  s /= scalar; v /= scalar;
  return *this;
}

template <typename T>
auto quaternion<T>::operator*=(T scalar) -> quaternion&
{
  s *= scalar; v *= scalar;
  return *this;
}

template <typename T>
constexpr auto operator+(quaternion<T> lhs, const quaternion<T>& rhs)
{
  lhs += rhs;
  return lhs;
}

template <typename T>
constexpr auto operator*(quaternion<T> lhs, const quaternion<T>& rhs)
{
  lhs *= rhs;
  return lhs;
}

template <typename T>
constexpr auto operator/(quaternion<T> q, T scalar)
{
  q /= scalar;
  return q;
}

template <typename T>
constexpr auto operator*(quaternion<T> q, T scalar)
{
  q *= scalar;
  return q;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const quaternion<T>& quat)
{
  std::size_t width = 0;
  std::stringstream w; w.precision(3);
  for (auto& vi : quat.v)
  {
    w.str(std::string{}); w.clear(); w << std::fixed << vi;
    width = std::max<std::size_t>(width, std::size_t(w.tellp()));
  }

  stream.precision(3);
  stream.setf(std::ios::fixed);
  for (auto& vi : quat.v)
  {
    stream << std::endl;
    stream.width(width); stream << vi;
  }

  stream << ' ';
  stream.width(width); stream << quat.s << std::endl;
  stream.width(width); stream << std::right << "(Vec)" << ' ';
  stream.width(width); stream << std::right << "(Sca)";
  return stream;
}

using quaternionf = quaternion<float>;
using quaterniond = quaternion<double>;

} // namespace ig

#endif // COMPUTE_QUATERNION_H
