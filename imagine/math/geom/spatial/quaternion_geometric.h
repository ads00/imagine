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

#ifndef IG_MATH_QUATERNION_GEOMETRIC_H
#define IG_MATH_QUATERNION_GEOMETRIC_H

namespace ig  {
namespace spt {

template <typename T>
constexpr auto dot(const quaternion<T>& lhs, const quaternion<T>& rhs) {
  return lhs.scalar * rhs.scalar + lin::dot(lhs.vector, rhs.vector);
}

template <typename T>
constexpr auto between(const quaternion<T>& lhs, const quaternion<T>& rhs) {
  return std::acos(dot(lhs, rhs));
}

template <typename T>
constexpr auto conj(const quaternion<T>& q) {
  return quaternion<T>{q.scalar, -q.vector};
}

template <typename T>
constexpr auto inv(const quaternion<T>& q) {
  return conj(q) / dot(q, q);
}

template <typename T>
constexpr auto norm(const quaternion<T>& q) {
  return std::sqrt(dot(q, q));
}

template <typename T>
auto normalise(const quaternion<T>& q) {
  auto nrm = norm(q);
  return quaternion<T>{q.scalar / nrm, q.vector / nrm};
}

template <typename T, typename Mat>
auto rotate(const quaternion<T>& q, const matrix_base<Mat>& v) {
  assert(v.vector() && v.vecsize() == 3 && "Quaternions can only rotate vectors in three-dimensional space");

  auto c1 = lin::cross(q.vector, v);
  auto c2 = lin::cross(q.vector, c1);
  return v + 2 * (c1 * q.scalar + c2);
}

} // namespace spt
} // namespace ig

#endif // IG_MATH_QUATERNION_GEOMETRIC_H
