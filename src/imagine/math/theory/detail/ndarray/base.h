/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYBASE_H
#define IG_MATH_NDARRAYBASE_H

#include "imagine/math/basis.h"
#include "imagine/math/theory/xpr.h"

#include <vector>

namespace ig {

template <typename T>
class ndarray;

// Meta
template <typename Xpr> struct ndarray_traits;
template <typename Xpr> struct ndarray_traits<const Xpr> : ndarray_traits<Xpr> {};

// Aliases
template <typename Array> using ndarray_t = typename ndarray_traits<Array>::value_type;
template <typename Array> using concrete_ndarray =
  ndarray
  <
    ndarray_t<Array>
  >;

template <typename D>
class ndarray_base : public xpr<D> {
public:
  using value_type = ndarray_t<D>;
  using index_type = std::initializer_list<uint32_t>;

  using base = xpr<D>;
  using base::derived;
  using base::eval;
  template <typename Xpr> using iterator = typename base::template iterator<Xpr>;

  auto begin() const { return iterator<const D>{derived(), 0}; }
  auto begin()       { return iterator<D>{derived(), 0}; }
  auto end() const   { return iterator<const D>{derived(), size()}; }
  auto end()         { return iterator<D>{derived(), size()}; }

  auto& shape() const { return derived().shape(); }

  auto dims() const { return derived().dims(); }
  auto size() const { return derived().size(); }

  decltype(auto) operator[](index_type index)
  { return derived()[index]; }

  auto& operator+=(value_type value)
  { return derived() = std::move(*this) + value; }
  auto& operator-=(value_type value)
  { return derived() = std::move(*this) - value; }
  auto& operator*=(value_type value)
  { return derived() = std::move(*this) * value; }
  auto& operator/=(value_type value)
  { return derived() = std::move(*this) / value; }

  template <typename Arr>
  auto& operator+=(const ndarray_base<Arr>& arr) { return derived() = std::move(*this) + arr; }
  template <typename Arr>
  auto& operator-=(const ndarray_base<Arr>& arr) { return derived() = std::move(*this) - arr; }
  template <typename Arr>
  auto& operator*=(const ndarray_base<Arr>& arr) { return derived() = std::move(*this) * arr; }
  template <typename Arr>
  auto& operator/=(const ndarray_base<Arr>& arr) { return derived() = std::move(*this) / arr; }
};

template <typename Gen, typename Arr>
void eval_helper(ndarray_base<Gen>& ev, const ndarray_base<Arr>& arr) {
  assert(
    ev.shape() == arr.shape()
    && "Incoherent array expression evaluation");

  for (size_t i = 0; i < ev.size(); ++i)
    ev [i] =
    arr[i];
}

template <typename Arr>
inline std::ostream& operator<<(std::ostream& stream, const ndarray_base<Arr>& arr) {
  size_t width = 0;
  std::stringstream w{}; w.precision(5);

  for (auto elemt : arr) {
    w.str(std::string{});
    w.clear();
    w << std::fixed << elemt; width = std::max<size_t>(width, size_t(w.tellp()));
  }

  stream.precision(5);
  stream << std::fixed << "ndarray []";
  return stream;
}

} // namespace ig

#endif // IG_MATH_NDARRAYBASE_H
