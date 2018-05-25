/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYSTATIC_H
#define IG_MATH_NDARRAYSTATIC_H

#include "imagine/math/basis.h"
#include <array>

namespace ig {
namespace nd {

template <typename T, size_t... D>
class static_storage {
public:
  using value_type = T;
  using container_type = std::array<value_type, (D * ... * 1)>;
  friend class ndarray<T, D...>;

  static_storage() = default;

  template <typename... Args>
  constexpr explicit static_storage(value_type i, Args&&... args) : buffer_{{i, std::forward<Args>(args)...}} {
    constexpr auto s = sizeof...(args);
    std::fill(
      buffer_.begin() + s + 1,
      buffer_.end(),
      buffer_[s]);
  }

  constexpr auto dims() const  { return sizeof...(D); }
  constexpr auto size() const  { return buffer_.size(); }

  constexpr auto buffer() const { return buffer_.data(); }
  constexpr auto buffer()       { return buffer_.data(); }

  constexpr decltype(auto) operator()(size_t n) const { return buffer_[n]; }
  constexpr decltype(auto) operator()(size_t n)       { return buffer_[n]; }

  constexpr auto operator[](uint32_t dimension) const;

protected:
  template <size_t C>
  constexpr auto stride() const;

  template <size_t C = 0, typename Id, typename... Ids> constexpr auto offset(Id i, Ids... is) const;
  template <size_t C>
  constexpr auto offset() const { return 0; }
  constexpr auto shape () const { return std::array{D...}; }

private:
  container_type buffer_{};
};

template
< typename t_,
  size_t... d_ >
template
< size_t C,
  typename Id,
  typename... Ids >
constexpr auto static_storage<t_, d_...>::offset(Id i, Ids... is) const {
  assert(
    C < dims() &&
    i < size() && "Invalid ndarray subscript");

  return
    stride<C>() * i +
    offset
    <C + 1>
      (is...);
}

template
< typename t_,
  size_t... d_ >
template
< size_t C >
constexpr auto static_storage<t_, d_...>::stride() const  {
  if constexpr (!C) {
    return 1;
  } else {
    return shape()[C - 1] * stride<C - 1>();
  }
}

template
< typename t_,
  size_t... d_ >
constexpr auto static_storage<t_, d_...>::operator[](uint32_t dimension) const {
  assert(dimension < dims() && "Invalid ndarray dimension");
  return shape()[dimension];
}

} // namespace nd
} // namespace ig

#endif // IG_MATH_NDARRAYSTATIC_H
