/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_QUATERNION_H
#define IG_MATH_QUATERNION_H

#include "imagine/math/theory/matrix.h"

namespace ig {

template <typename Arithmetic>
class quaternion {
public:
  using value_type = Arithmetic;
  using vector_type = colvec<value_type, 3>;

  quaternion() = default;
  explicit quaternion(
    value_type sca, 
    const vector_type& vec)
    : scalar{sca}
    , vector{vec} {}

  static auto eye()
  { return quaternion{1, vector_type{}}; }
  static auto axis(value_type angle, const vector_type& axis)
  { return quaternion{std::cos(angle / 2), axis * std::sin(angle / 2)}; }
  static auto euler(value_type roll, value_type pitch, value_type yaw) {
    auto cr = std::cos(roll),  sr = std::sin(roll);
    auto cp = std::cos(pitch), sp = std::sin(pitch);
    auto cy = std::cos(yaw),   sy = std::sin(yaw);

    return quaternion{cr * cp * cy - sr * sp * sy, vector_type{cr * cp * cy + sr * sp * sy,
                                                               sr * cp * cy + cr * sp * sy,
                                                               cr * sp * cy - sr * cp * sy}};
  }

  value_type scalar;
  vector_type vector;
};

template <typename Arithmetic>
auto operator+(const quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return quaternion<Arithmetic>{lhs.scalar + rhs.scalar, lhs.vector + rhs.vector}; }

template <typename Arithmetic>
auto operator-(const quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return quaternion<Arithmetic>{lhs.scalar - rhs.scalar, lhs.vector - rhs.vector}; }

template <typename Arithmetic>
auto operator*(const quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs) {
  return quaternion<Arithmetic>{lhs.scalar * rhs.scalar - lin::dot(lhs.vector, rhs.vector),
                                lhs.scalar * rhs.vector + rhs.scalar * lhs.vector + lin::cross(lhs.vector, rhs.vector)};
}

template <typename Arithmetic>
auto operator*(const quaternion<Arithmetic>& lhs, Arithmetic rhs)
{ return quaternion<Arithmetic>{lhs.scalar * rhs, lhs.vector * rhs}; }

template <typename Arithmetic>
auto operator*(Arithmetic rhs, const quaternion<Arithmetic>& lhs)
{ return quaternion<Arithmetic>{rhs * lhs.scalar, rhs * lhs.vector}; }

template <typename Arithmetic>
auto operator/(const quaternion<Arithmetic>& lhs, Arithmetic rhs)
{ return quaternion<Arithmetic>{lhs.scalar / rhs, lhs.vector / rhs}; }

template <typename Arithmetic>
auto& operator+=(quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return lhs = lhs + rhs; }

template <typename Arithmetic>
auto& operator-=(quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return lhs = lhs - rhs; }

template <typename Arithmetic>
auto& operator*=(quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return lhs = lhs * rhs; }

template <typename Arithmetic>
auto& operator*=(quaternion<Arithmetic>& lhs, Arithmetic rhs)
{ return lhs = lhs * rhs; }

template <typename Arithmetic>
auto& operator/=(quaternion<Arithmetic>& lhs, const quaternion<Arithmetic>& rhs)
{ return lhs = lhs / rhs; }

template <typename Arithmetic>
inline std::ostream& operator<<(std::ostream& stream, const quaternion<Arithmetic>& quat) {
  size_t width = 0;
  std::stringstream w{}; w.precision(5);

  for (auto velemt : quat.vector) {
    w.str(std::string{});
    w.clear();
    w << std::fixed << velemt; width = std::max<size_t>(width, size_t(w.tellp()));
  }

  stream.precision(5);
  stream << std::fixed;
  for (auto velemt : quat.vector) {
    stream << std::endl;
    stream.width(width); stream << velemt; }

  stream << ' ';
  stream.width(width); stream << quat.scalar << std::endl;
  stream.width(width); stream << std::right << "(Vec)" << ' ';
  stream.width(width); stream << std::right << "(Sca)";
  return stream;
}

} // namespace ig
#include "imagine/math/geom/spatial/quaternion_geometric.h"

#endif // IG_MATH_QUATERNION_H
