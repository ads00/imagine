/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYBASE_H
#define IG_MATH_NDARRAYBASE_H

#include "imagine/math/basis.h"
#include "imagine/math/theory/detail/relational.h"

namespace ig {

template <typename T, size_t... D>
class ndarray;

template <typename F, typename... Xprs>
class wise;

template <typename Xpr, typename Shape>
class cast;

template <typename Xpr>
class view;

// Meta
template <typename Xpr> struct ndarray_traits;
template <typename Xpr> struct ndarray_traits<const Xpr> : ndarray_traits<Xpr> {};

// Aliases
template <typename Array> using ndarray_t = typename ndarray_traits< std::decay_t<Array> >::value_type;
template <typename Array> using concrete_ndarray =
  ndarray
  <
  ndarray_t<Array>
  >;

template <typename D>
class ndarray_base : public xpr<D> {
public:
  using value_type = ndarray_t<D>;

  using base = xpr<D>;
  using base::derived;

  auto size() const { return derived().size(); }
  auto dims() const { return derived().dims(); }
  auto shape() const -> decltype(auto) { return derived().shape(); }

  bool is_vector() const { return dims() == 1; }
  bool is_matrix() const { return dims() == 2; }
  bool is_tensor() const { return dims() >= 3; }

  bool is_balanced() const;

  template <typename... Id> decltype(auto) operator()(Id... ids) const { return derived()(ids...); }
  template <typename... Id> decltype(auto) operator()(Id... ids)       { return derived()(ids...); }

  auto operator[](uint32_t dimension) const
  { return shape()[dimension]; }

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

template <typename D>
bool ndarray_base<D>::is_balanced() const {
  auto s = shape();
  return std::equal(
    s.begin() + 1,
    s.end(),
    s.begin());
}

template <typename Gen, typename Arr>
void eval_helper(ndarray_base<Gen>& ev, const ndarray_base<Arr>& arr) {
  assert(
    ev.dims() == arr.dims() &&
    ev.size() == arr.size() && "Incoherent ndarray expression evaluation");

  for (size_t i = 0; i < ev.size(); ++i)
    ev (i) =
    arr(i);
}

template <typename Arr>
inline std::ostream& operator<<(std::ostream& stream, const ndarray_base<Arr>& arr) {
  size_t width = 0;
  std::stringstream w{}; w.precision(5);

  /*
  for (auto elemt : arr) {
    w.str(std::string{});
    w.clear();
    w << std::fixed << elemt; width = std::max<size_t>(width, size_t(w.tellp()));
  }
  */

  stream.precision(5);
  stream << std::fixed << "ndarray []";
  return stream;
}

} // namespace ig

#endif // IG_MATH_NDARRAYBASE_H
