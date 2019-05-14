/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYTRANS_H
#define IG_MATH_NDARRAYTRANS_H

#include "imagine/math/theory/detail/ndarray/view.h"

namespace ig {

template <typename Xpr, typename Shp, typename Str>
struct ndarray_traits
<
  transpose<Xpr, Shp, Str>
>
{
  using value_type = ndarray_t<Xpr>;
};

template 
< typename x_,
  typename sh_,
  typename st_ >
class transpose : public ndarray_base< transpose<x_, sh_, st_> > {
public:
  explicit constexpr transpose(x_ xpr, const sh_& shape, const st_& strides)
    : xpr_{xpr}
    , shape_{shape}
    , strides_{strides} {

    std::reverse(
      shape_.begin(), 
      shape_.end());
    std::reverse(
      strides_.begin(), 
      strides_.end());
  }

  auto size() const { return xpr_.size(); }
  auto dims() const { return xpr_.dims(); }
  auto& shape() const   { return shape_; }
  auto& strides() const { return strides_; }

  template <typename... Id> 
  decltype(auto) operator()(Id... ids) const { return std::apply([&](auto&&... x) { return xpr_(x...); }, reverse(std::forward_as_tuple(ids...))); }
  template <typename... Id> 
  decltype(auto) operator()(Id... ids)       { return std::apply([&](auto&&... x) { return xpr_(x...); }, reverse(std::forward_as_tuple(ids...))); }

  decltype(auto) eval(size_t id) const {
    return eval_strided(
      [this](auto i) { return i < dims() - 1; },
      *this, 
       xpr_,
       strides_,
       id);
  }

private:
  x_ xpr_;
  sh_ shape_;
  st_ strides_;
};

// deduction
template 
< typename x_, 
  typename sh_, 
  typename st_ > 
  explicit transpose(x_ xpr, const sh_& shape, const st_& stride) 
        -> transpose< std::conditional_t<std::is_reference_v<x_>, x_, x_&>, sh_, st_ >;

} // namespace ig

#endif // IG_MATH_NDARRAYTRANS_H
