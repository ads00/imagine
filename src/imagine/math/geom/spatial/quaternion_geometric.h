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

template <typename Arithmetic>
constexpr auto dot(const quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return lhs.scalar * rhs.scalar + lin::dot(lhs.vector, rhs.vector); }

template <typename Arithmetic>
constexpr auto between(const quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return std::acos(dot(lhs, rhs)); }

template <typename Arithmetic>
constexpr auto conj(const quaternion<Arithmetic>& q)
{ return quaternion<Arithmetic>{q.scalar, -q.vector}; }

template <typename Arithmetic>
constexpr auto inv(const quaternion<Arithmetic>& q)
{ return conj(q) / dot(q, q); }

template <typename Arithmetic>
constexpr auto norm(const quaternion<Arithmetic>& q)
{ return std::sqrt(dot(q, q)); }

template <typename Arithmetic>
constexpr auto normalise(const quaternion<Arithmetic>& q)
{ return q / norm(q); }

template <typename Arithmetic>
auto slerp(const quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs, Arithmetic t) {
  auto coshalf = dot(lhs, rhs);
  if (std::abs(coshalf) >= 1) {
    return lerp(lhs, rhs, t);
  } else {
    auto a = std::acos(coshalf);
    return (lhs * std::sin((1 - t) * a) + rhs * std::sin(t * a)) / std::sin(a);
  }
}

template <typename Arithmetic, typename Mat>
auto rotate(const quaternion<Arithmetic>& q, const matrix_base<Mat>& v) {
  assert(
    v.vector() && 
    v.vecsize() == 3 
    && "Quaternions can only rotate vectors in three-dimensional space");

  auto w = q.scalar * v + lin::cross(q.vector, v);
  return lin::dot(q.vector, v) * q.vector + q.scalar * w + lin::cross(w, -q.vector);
}

} // namespace spt
} // namespace ig

#endif // IG_MATH_QUATERNION_GEOMETRIC_H
