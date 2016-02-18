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

#ifndef COMPUTE_GEOMETRIC_H
#define COMPUTE_GEOMETRIC_H

#include "imagine/compute/linalg/matrix.h"
#include "imagine/compute/linalg/quaternion.h"

namespace ig     {
namespace linalg {

template <typename T>
constexpr auto dot(const quaternion<T>& lhs, const quaternion<T>& rhs)
{
  return lhs.s*rhs.s + linalg::dot(lhs.v, rhs.v);
}

template <typename T>
constexpr auto between(const quaternion<T>& lhs, const quaternion<T>& rhs)
{
  return std::acos(dot(lhs, rhs));
}

template <typename T>
constexpr auto inv(const quaternion<T>& q)
{
  return conj(q) / dot(q, q);
}

template <typename T>
constexpr auto norm(const quaternion<T>& q)
{
  return std::sqrt(dot(q, q));
}

template <typename T>
constexpr auto conj(const quaternion<T>& q) -> quaternion<T>
{
  return { q.s, { -q.v[0], -q.v[1], -q.v[2] } };
}

template <typename T>
auto normalise(const quaternion<T>& q) -> quaternion<T>
{
  auto nrm = norm(q);
  return { q.s / nrm, { q.v[0] / nrm, q.v[1] / nrm, q.v[2] / nrm } };
}

template <typename T, typename Alg>
auto rotate(const quaternion<T>& q, const alg<Alg>& v)
-> vector<std::common_type_t< T, alg_t<Alg> >, 3>
{
  assert(v.vector() && v.vecsize() == 3
         && "Quaternion can only rotate vectors in three-dimensional space");

  const auto c1 = linalg::cross(q.v, v);
  const auto c2 = linalg::cross(q.v, c1);

  return v + T(2.0)*(c1*q.s + c2);
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_GEOMETRIC_H
