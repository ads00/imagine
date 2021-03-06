/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_AABB_H
#define IG_MATH_AABB_H

#include "imagine/math/theory/matrix.h"

namespace ig {

template <typename Arithmetic>
class aabb {
public:
  using value_type = Arithmetic;
  using vector_type = colvec<value_type, 3>;

  aabb() { invalidate(); }
  explicit aabb(
    const vector_type& min, 
    const vector_type& max)
    : min{min}
    , max{max} {}
  explicit aabb(const std::vector<vector_type>& points) { invalidate(); for (auto& point : points) expand(point); }

  void invalidate();
  void expand(const vector_type& p);
  void expand(const aabb& o);

  auto half_area() const {
    auto ext = extent();
    return ext[0] * ext[1] + ext[0] * ext[2] + ext[1] * ext[2]; }
  auto area() const { return 2 * half_area(); }

  auto centroid() const { return (min + max) * value_type(0.5); }
  auto extent() const   { return max - min; }
  auto volume() const   { return extent().prod(); }

  vector_type min, max;
};

template <typename Arithmetic>
void aabb<Arithmetic>::invalidate()
{ for (uint32_t i = 0; i < 3; ++i) min[i] = std::numeric_limits<Arithmetic>::infinity(), max[i] = -std::numeric_limits<Arithmetic>::infinity(); }

template <typename Arithmetic>
void aabb<Arithmetic>::expand(const vector_type& p)
{ for (uint32_t i = 0; i < 3; ++i) min[i] = std::min(min[i], p[i]), max[i] = std::max(max[i], p[i]); }

template <typename Arithmetic>
void aabb<Arithmetic>::expand(const aabb& o)
{ min = minima(min, o.min); max = maxima(max, o.max); }

} // namespace ig

#endif // IG_MATH_AABB_H
