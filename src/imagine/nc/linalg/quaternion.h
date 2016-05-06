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

#ifndef IG_NC_QUATERNION_H
#define IG_NC_QUATERNION_H

#include "imagine/nc/linalg/matrix.h"

namespace ig {

template <typename T>
class quaternion {
public:
  using vec_t = vector<T, 3>;

  constexpr quaternion() : sca_{1}, vec_{} {}
  constexpr quaternion(T sca, const vec_t& vec)
    : sca_{sca}, vec_{vec} {}

  auto operator+=(const quaternion& q) -> quaternion&;
  auto operator*=(const quaternion& q) -> quaternion&;

  auto operator/=(T scalar) -> quaternion&; auto operator*=(T scalar) -> quaternion&;

  static quaternion axis(T angle, const vec_t& axis) {
    return {std::cos(angle / T(2)), axis * std::sin(angle / T(2))};
  }

  static quaternion euler(T roll, T pitch, T yaw) {
    const T cr = std::cos(roll);  const T sr = std::sin(roll);
    const T cp = std::cos(pitch); const T sp = std::sin(pitch);
    const T cy = std::cos(yaw);   const T sy = std::sin(yaw);

    return {cr*cp*cy - sr*sp*sy, {cr*cp*cy + sr*sp*sy,
                                  sr*cp*cy + cr*sp*sy,
                                  cr*sp*cy - sr*cp*sy}};
  }

  T sca_;
  vec_t vec_;
};

template <typename T>
auto quaternion<T>::operator+=(const quaternion& q) -> quaternion& {
  sca_ += q.sca_; vec_ += q.vec_;
  return *this;
}

template <typename T>
auto quaternion<T>::operator*=(const quaternion& q) -> quaternion& {
  auto&& p = *this;
  sca_ = p.sca_*q.sca_ - linalg::dot(p.vec_, q.vec_);
  vec_ = p.sca_*q.vec_ + q.sca_*p.vec_ + linalg::cross(p.vec_, q.vec_);
  return *this;
}

template <typename T>
auto quaternion<T>::operator/=(T scalar) -> quaternion& {
  sca_ /= scalar; vec_ /= scalar;
  return *this;
}

template <typename T>
auto quaternion<T>::operator*=(T scalar) -> quaternion& {
  sca_ *= scalar; vec_ *= scalar;
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
  auto width = size_t{0};
  auto w = std::stringstream{}; w.precision(3);

  for (auto&& vi : quat.vec_) {
    w.str(std::string{}); w.clear(); w << std::fixed << vi;
    width = std::max<size_t>(width, size_t(w.tellp()));
  }

  stream.precision(3); stream.setf(std::ios::fixed);
  for (auto&& vi : quat.vec_) {
    stream << std::endl;
    stream.width(width); stream << vi;
  }

  stream << ' ';
  stream.width(width); stream << quat.sca_ << std::endl;
  stream.width(width); stream << std::right << "(Vec)" << ' ';
  stream.width(width); stream << std::right << "(Sca)";
  return stream;
}

using quaternionf = quaternion<float>;
using quaterniond = quaternion<double>;

} // namespace ig

#endif // IG_NC_QUATERNION_H
