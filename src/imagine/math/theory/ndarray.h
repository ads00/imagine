/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAY_H
#define IG_MATH_NDARRAY_H

#include "imagine/math/theory/detail/ndarray/base.h"

#include "imagine/math/theory/detail/ndarray/allocator/static.h"
#include "imagine/math/theory/detail/ndarray/allocator/dynamic.h"

namespace ig {

template <typename T,  size_t... D>
struct ndarray_traits
<
  ndarray<T, D...>
>
{
  using value_type = T;
};

template <typename t_, size_t... d_>
class ndarray : public ndarray_base< ndarray<t_, d_...> > {
public:
  using value_type = t_;
  static constexpr auto dynamic = !sizeof...(d_);
  static constexpr auto immutable = !dynamic;

  template
  < bool X = immutable,
    typename = std::enable_if_t<X>,
    typename... Args >
  constexpr explicit ndarray(value_type i, Args&&... args)
    : storage_{i, std::forward<Args>(args)...} {}

  template
  < bool X = dynamic,
    typename = std::enable_if_t<X> >
  constexpr explicit ndarray(std::initializer_list<size_t> dims)
    : storage_{{dims}} {}

  template <typename Arr>
  ndarray(const ndarray_base<Arr>& o) : ndarray{o, o.shape(), std::integral_constant<bool, immutable>{}}
  { eval_helper(*this, o); }

  template <typename Arr, typename Shape> ndarray(const ndarray_base<Arr>& o, const Shape& shape, std::true_type) {}
  template <typename Arr, typename Shape> ndarray(const ndarray_base<Arr>& o, const Shape& shape, std::false_type)
  : storage_{{
    shape.begin(),
    shape.end  ()}} {}

  auto size() const { return storage_.size(); }
  auto dims() const { return storage_.dims(); }
  auto shape() const -> decltype(auto) { return storage_.shape(); }

  auto buffer() const { return storage_.buffer(); }
  auto buffer()       { return storage_.buffer(); }

  auto operator->() const { return &storage_; }
  auto operator->()       { return &storage_; }

  template <typename... Id> decltype(auto) operator()(Id... ids) const { return storage_(layout(ids...)); }
  template <typename... Id> decltype(auto) operator()(Id... ids)       { return storage_(layout(ids...)); }

protected:
  template <typename... Id>
  auto layout(Id... ids) const
  { return std::apply([this](auto&&... x) { return storage_.offset(x...); }, /*reverse*/(std::make_tuple(ids...))); }

protected:
  std::conditional_t
  < dynamic,
    nd::dynamic_storage<value_type>, nd::static_storage<value_type, d_...>
  > storage_;
};

} // namespace ig

#endif // IG_MATH_NDARRAY_H
