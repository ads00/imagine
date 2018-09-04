/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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
    v.vec_size() == 3
    && "Quaternions can only rotate vectors in three-dimensional space");

  auto w = q.scalar * v + lin::cross(q.vector, v);
  return lin::dot(q.vector, v) * q.vector + q.scalar * w + lin::cross(w, -q.vector);
}

} // namespace spt
} // namespace ig

#endif // IG_MATH_QUATERNION_GEOMETRIC_H
