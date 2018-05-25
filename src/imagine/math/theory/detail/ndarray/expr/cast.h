/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYCAST_H
#define IG_MATH_NDARRAYCAST_H

#include "imagine/math/theory/detail/ndarray/base.h"

namespace ig {

template <typename Xpr, typename Shape>
struct ndarray_traits
<
  cast<Xpr, Shape>
>
{
  using value_type = ndarray_t<Xpr>;
};

template
< typename x_,
  typename s_ >
class cast : public ndarray_base< cast<x_, s_> > {
public:
  explicit cast(const x_& xpr, const s_& shape)
    : xpr_{xpr}
    , shape_{shape}
    , size_{
      std::accumulate(
        shape_.begin(),
        shape_.end(),
        size_t(1),
        std::multiplies<>{})} {}

  auto size() const { return size_; }
  auto dims() const { return shape_.size(); }
  auto& shape() const { return shape_; }

  template <typename... Id> decltype(auto) operator()(Id... ids) const { return xpr_(ids % xpr_.size()...); }
  template <typename... Id> decltype(auto) operator()(Id... ids)       { return xpr_(ids % xpr_.size()...); }

private:
  x_ xpr_;
  s_ shape_; size_t size_;
};

template <typename x_, typename s_> cast(x_&, const s_&) -> cast<x_&, s_>;

namespace nd {

template <typename Xpr, typename Shape>
constexpr auto broadcast(Xpr&& xpr, const Shape& shape) {
  auto valid_cast = [xs = xpr.shape(), &shape] {
    return
    std::equal(
      xs.begin(),
      xs.end(),
      shape.begin(),
      [](auto& lhs, auto& rhs) {
        return lhs == rhs ||
               lhs == 1   ||
               rhs == 1; }); };
  assert(
    valid_cast() &&
    "Invalid ndarray broadcasting, dimensions must be either equal or one of them must be one");
  return cast{xpr.derived(), shape};
}

} // namespace nd
} // namespace ig

#endif // IG_MATH_NDARRAYCAST_H
