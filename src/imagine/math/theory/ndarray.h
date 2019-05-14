/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAY_H
#define IG_MATH_NDARRAY_H

#include "imagine/math/theory/detail/ndarray/base.h"
#include "imagine/math/theory/detail/ndarray/view.h"

#include "imagine/math/theory/detail/ndarray/expr/trans.h"
#include "imagine/math/theory/detail/ndarray/expr/wise.h"

#include "imagine/math/theory/detail/ndarray/allocator/dense.h"
#include "imagine/math/theory/detail/ndarray/allocator/sparse.h"

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

  constexpr ndarray() = default;

  template
  < typename Arg,
    typename... Args,
    typename = std::enable_if_t<!is_ndarray<Arg>::value> >
  constexpr explicit ndarray(Arg&& arg, Args&&... args) : storage_{arg, args...} {}

  template <typename Arr>
  ndarray(const ndarray_base<Arr>& o) : ndarray{o, o.shape(), std::integral_constant<bool, immutable>{}}
  { eval_helper(*this, o); }

  template <typename Arr, typename Shape> ndarray(const ndarray_base<Arr>& o, const Shape& shape, std::true_type) : storage_{} {}
  template <typename Arr, typename Shape> ndarray(const ndarray_base<Arr>& o, const Shape& shape, std::false_type)
  : storage_{shape} {}

  auto size() const { return storage_.size(); }
  auto dims() const { return storage_.dims(); }
  auto shape() const   -> decltype(auto) { return storage_.shape(); }
  auto strides() const -> decltype(auto) { return storage_.strides(); }

  auto data() const { return storage_.buffer(); }
  auto data()       { return storage_.buffer(); }

  auto operator->() const { return &storage_; }
  auto operator->()       { return &storage_; }

  template <typename... Id> decltype(auto) operator()(Id... ids) const { return storage_(layout(ids...)); }
  template <typename... Id> decltype(auto) operator()(Id... ids)       { return storage_(layout(ids...)); }

  decltype(auto) eval(size_t id) const 
  { return storage_(id); }
  decltype(auto) eval(size_t id)       
  { return storage_(id); }

  template <typename Arr>
  auto& operator=(const ndarray_base<Arr>&  o) { return eval_helper(*this, o); }
  template <typename Arr>
  auto& operator=(      ndarray_base<Arr>&& o) { return eval_helper(*this, o); }

  template <typename Shape> 
  static auto identity(Shape&& shape);

protected:
  template <typename... Id>
  auto layout(Id... ids) const
  { return storage_.access(ids...); }

protected:
  nd::dense
  < 
    std::conditional_t
    < dynamic,
      nd::dynamic_alloc<value_type>,
      nd:: static_alloc<value_type, d_...>
    >
  > storage_;
};

template 
<typename t_, size_t... d_>
template 
<typename Shape>
auto ndarray<t_, d_...>::identity(Shape&& shape) { 
  ndarray I{shape};
  for (auto it = I.begin(); it < I.end(); it += I.strides().back() + 1) (*it) = 1;
  return I; 
}

} // namespace ig

#endif // IG_MATH_NDARRAY_H
