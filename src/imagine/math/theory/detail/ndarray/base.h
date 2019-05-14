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

template <typename D> 
class ndarray_base;
template <typename T, size_t... D> 
class ndarray;

template <typename Spn> 
class view_ranges;

template <typename Xpr, typename Shp, typename Rng> class view;
template <typename Xpr, typename Shp, typename Str> class domain;
template <typename Xpr, typename Shp, typename Str> class transpose;

template <typename Fn, typename Sen, typename... Xprs> class wise;

// Meta
template <typename Xpr> struct ndarray_traits;
template <typename Xpr> struct ndarray_traits<const Xpr> : ndarray_traits<Xpr> {};

template <typename Xpr>
struct is_ndarray : std::is_base_of< ndarray_base< std::decay_t<Xpr> >, std::decay_t<Xpr> >{};

// Aliases
template <typename Array> using ndarray_t = typename ndarray_traits< std::decay_t<Array> >::value_type;

template <typename D>
class ndarray_base : public xpr<D> {
public:
  using value_type = ndarray_t<D>;

  using base = xpr<D>;
  using base::derived;
  //using base::begin;
  //using base::end;

  auto begin() const { return base::ndbegin(); }
  auto begin()       { return base::ndbegin(); }
  auto end() const   { return base::ndend(); }
  auto end()         { return base::ndend(); }

  auto size() const { return derived().size(); }
  auto dims() const { return derived().dims(); }
  auto& shape() const   { return derived().shape(); }
  auto& strides() const { return derived().strides(); }

  bool is_vector() const { return dims() == 1; }
  bool is_matrix() const { return dims() == 2; }
  bool is_tensor() const { return dims() >= 3; }

  bool is_balanced() const;

  auto t() const { return transpose{derived(), shape(), strides()}; }
  auto t()       { return transpose{derived(), shape(), strides()}; }
  //auto r() const {}
  //auto r()       {}
  //auto diag(size_t axis = 2) const {}
  //auto diag(size_t axis = 2)       {}

  template <typename... Id>
  decltype(auto) operator()(Id... ids) const { return derived()(ids...); }
  template <typename... Id>
  decltype(auto) operator()(Id... ids)       { return derived()(ids...); }

  template <typename Spans>
  auto operator[](view_ranges<Spans>&& range) const { return view{derived(), shape(), range}; }
  template <typename Spans>
  auto operator[](view_ranges<Spans>&& range)       { return view{derived(), shape(), range}; }

  auto operator[](size_t dimension) const
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

  auto sum() const  -> value_type;
  auto prod() const -> value_type;
};

template <typename D>
bool ndarray_base<D>::is_balanced() const {
  auto& s = shape();
  return std::equal(
    s.begin() + 1,
    s.end(),
    s.begin());
}

template <typename D>
auto ndarray_base<D>::sum() const -> value_type
{ return std::accumulate(begin(), end(), value_type(0)); }

template <typename D>
auto ndarray_base<D>::prod() const -> value_type
{ return std::accumulate(begin(), end(), value_type(1), std::multiplies<>{}); }

template 
< typename Gen, 
  typename Arr >
decltype(auto) eval_helper(ndarray_base<Gen>& ev, const ndarray_base<Arr>& arr) {
  assert(
    ev.dims() == 
    arr.dims() &&
    ev.size() == 
    arr.size() && "Incoherent ndarray expression evaluation");

  std::transform(
    arr.begin(), 
    arr.end(), 
    ev.begin(), 
    [](auto&& element) { return element; });
  return ev;
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
