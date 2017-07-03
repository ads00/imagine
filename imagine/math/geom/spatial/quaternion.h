/*
 Copyright (c) 2017
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

#ifndef IG_MATH_QUATERNION_H
#define IG_MATH_QUATERNION_H

#include "imagine/math/theory/matrix.h"

namespace ig {

template <typename T>
class quaternion {
public:
  using vector_type = colvec<T, 3>;

  quaternion() 
    : scalar{1}
    , vector{} {}
  explicit quaternion(T sca, const vector_type& vec)
    : scalar{sca}
    , vector{vec} {}

  auto operator+=(const quaternion& q) -> quaternion&;
  auto operator*=(const quaternion& q) -> quaternion&;

  auto operator/=(T sca) -> quaternion&; 
  auto operator*=(T sca) -> quaternion&;

  static auto axis(T angle, const vector_type& axis) {
    return quaternion{std::cos(angle / 2), axis * std::sin(angle / 2)};
  }

  static auto euler(T roll, T pitch, T yaw) {
    auto cr = std::cos(roll),  sr = std::sin(roll);
    auto cp = std::cos(pitch), sp = std::sin(pitch);
    auto cy = std::cos(yaw),   sy = std::sin(yaw);

    return quaternion{cr * cp * cy - sr * sp * sy, 
      vector_type{cr * cp * cy + sr * sp * sy,
                  sr * cp * cy + cr * sp * sy,
                  cr * sp * cy - sr * cp * sy}};
  }

  T scalar;
  vector_type vector;
};

template <typename T>
auto quaternion<T>::operator+=(const quaternion& q) -> quaternion& {
  scalar += q.scalar; vector += q.vector;
  return *this;
}

template <typename T>
auto quaternion<T>::operator*=(const quaternion& q) -> quaternion& {
  auto& s = *this;
  scalar = s.scalar * q.scalar - lin::dot(s.vector, q.vector);
  vector = s.scalar * q.vector + q.scalar * s.vector + lin::cross(s.vector, q.vector);
  return *this;
}

template <typename T>
auto quaternion<T>::operator/=(T sca) -> quaternion& {
  scalar /= sca; vector /= sca;
  return *this;
}

template <typename T>
auto quaternion<T>::operator*=(T sca) -> quaternion& {
  scalar *= sca; vector *= sca;
  return *this;
}

template <typename T>
constexpr auto operator+(quaternion<T> lhs, const quaternion<T>& rhs) {
  lhs += rhs;
  return lhs;
}

template <typename T>
constexpr auto operator*(quaternion<T> lhs, const quaternion<T>& rhs) {
  lhs *= rhs;
  return lhs;
}

template <typename T>
constexpr auto operator/(quaternion<T> q, T scalar) {
  q /= scalar;
  return q;
}

template <typename T>
constexpr auto operator*(quaternion<T> q, T scalar) {
  q *= scalar;
  return q;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const quaternion<T>& quat) {
  size_t width = 0;
  std::stringstream 
    w{}; w.precision(3);

  for (auto velemt : quat.vec_) {
    w.str(std::string{}); w.clear(); w << std::fixed << velemt;
    width = std::max<size_t>(width, size_t(w.tellp()));
  }

  stream.precision(3); stream.setf(std::ios::fixed);
  for (auto velemt : quat.vector) {
    stream << std::endl;
    stream.width(width); stream << velemt;
  }

  stream << ' ';
  stream.width(width); stream << quat.scalar << std::endl;
  stream.width(width); stream << std::right << "(Vec)" << ' ';
  stream.width(width); stream << std::right << "(Sca)";
  return stream;
}

using quaternionf = quaternion<float>;
using quaterniond = quaternion<double>;

} // namespace ig

#include "imagine/math/geom/spatial/quaternion_geometric.h"
#include "imagine/math/geom/spatial/quaternion_interpolate.h"

#endif // IG_MATH_QUATERNION_H
