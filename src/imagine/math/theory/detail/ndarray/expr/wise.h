/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYWISE_H
#define IG_MATH_NDARRAYWISE_H

#include "imagine/math/theory/detail/ndarray/base.h"

namespace ig {

template <typename Fn, typename Sen, typename... Xprs>
struct ndarray_traits
<
  wise<Fn, Sen, Xprs...>
>
{
  using value_type = std::common_type_t<ndarray_t<Sen>, ndarray_t<Xprs>...>;
};

template
< typename f_,
  typename s_,
  typename... x_ >
class wise : public ndarray_base< wise<f_, s_, x_...> > {
public:
  explicit constexpr wise(const f_& op, s_ sen, x_... xprs)
    : op_{op}
    , sen_{sen}
    , xprs_{std::forward_as_tuple(xprs...)} {

      std::apply(
        [this](auto&&... x) {
          assert(
            (std::equal(
              sen_.shape().begin(), 
              sen_.shape().end(), 
              x.shape().begin()) && 
            ...) && "Invalid ndarray wise operation");
        }, 
        xprs_);
    }

  auto size() const { return sen_.size(); }
  auto dims() const { return sen_.dims(); }
  auto& shape() const   { return sen_.shape(); }
  auto& strides() const { return sen_.strides(); }

  template <typename... Id> 
  auto operator()(Id... ids) const 
  { return std::apply([&](auto&&... x) { return op_(sen_(ids...), x(ids...)...); }, xprs_); }

  auto eval(size_t id) const 
  { return std::apply([&](auto&&... x) { return op_(sen_.eval(id), x.eval(id)...); }, xprs_); }

private:
  f_ op_;
  s_ sen_;
  std::tuple<x_...> 
  xprs_;
};

// deduction
template 
< typename f_, 
  typename s_, 
  typename... x_ > 
  explicit wise(const f_& op, s_ sen, x_... xprs) 
        -> wise< f_, std::conditional_t<std::is_reference_v<s_>, const s_, const s_&>, std::conditional_t<std::is_reference_v<x_>, const x_, const x_&>... >;

// Unary operators
template <typename Xpr>
constexpr auto operator-(const ndarray_base<Xpr>& xpr) { return wise{std::negate<>{}, xpr.derived()}; }

// Binary operators
template <typename Lhs, typename Rhs>
constexpr auto operator+(const ndarray_base<Lhs>& lhs, const ndarray_base<Rhs>& rhs)
{ return wise{std::plus<>{}, lhs.derived(), rhs.derived()}; }

template <typename Lhs, typename Rhs>
constexpr auto operator-(const ndarray_base<Lhs>& lhs, const ndarray_base<Rhs>& rhs)
{ return wise{std::minus<>{}, lhs.derived(), rhs.derived()}; }

template <typename Lhs, typename Rhs>
constexpr auto operator*(const ndarray_base<Lhs>& lhs, const ndarray_base<Rhs>& rhs)
{ return wise{std::multiplies<>{}, lhs.derived(), rhs.derived()}; }

template <typename Lhs, typename Rhs>
constexpr auto operator/(const ndarray_base<Lhs>& lhs, const ndarray_base<Rhs>& rhs)
{ return wise{std::divides<>{}, lhs.derived(), rhs.derived()}; }

// Scalars
template <typename S, typename Xpr>
using swise =
  std::enable_if_t<
    std::is_scalar_v<S> ||
     (std::is_same_v<ndarray_t<Xpr>, S> && std::is_trivially_copyable_v<S>)
  >;

template < typename Lhs, typename S, typename = swise<S, Lhs> > constexpr auto operator+(const ndarray_base<Lhs>& lhs, S rhs) { return wise{[rhs](auto&& x) { return x + rhs; }, lhs.derived()}; }
template < typename Rhs, typename S, typename = swise<S, Rhs> > constexpr auto operator+(S lhs, const ndarray_base<Rhs>& rhs) { return wise{[lhs](auto&& x) { return lhs + x; }, rhs.derived()}; }

template < typename Lhs, typename S, typename = swise<S, Lhs> > constexpr auto operator-(const ndarray_base<Lhs>& lhs, S rhs) { return wise{[rhs](auto&& x) { return x - rhs; }, lhs.derived()}; }
template < typename Rhs, typename S, typename = swise<S, Rhs> > constexpr auto operator-(S lhs, const ndarray_base<Rhs>& rhs) { return wise{[lhs](auto&& x) { return lhs - x; }, rhs.derived()}; }

template < typename Lhs, typename S, typename = swise<S, Lhs> > constexpr auto operator*(const ndarray_base<Lhs>& lhs, S rhs) { return wise{[rhs](auto&& x) { return x * rhs; }, lhs.derived()}; }
template < typename Rhs, typename S, typename = swise<S, Rhs> > constexpr auto operator*(S lhs, const ndarray_base<Rhs>& rhs) { return wise{[lhs](auto&& x) { return lhs * x; }, rhs.derived()}; }

template < typename Lhs, typename S, typename = swise<S, Lhs> > constexpr auto operator/(const ndarray_base<Lhs>& lhs, S rhs) { return wise{[rhs](auto&& x) { return x / rhs; }, lhs.derived()}; }
template < typename Rhs, typename S, typename = swise<S, Rhs> > constexpr auto operator/(S lhs, const ndarray_base<Rhs>& rhs) { return wise{[lhs](auto&& x) { return lhs / x; }, rhs.derived()}; }

} // namespace ig

#endif // IG_MATH_NDARRAYWISE_H
