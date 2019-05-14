/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYVIEW_H
#define IG_MATH_NDARRAYVIEW_H

#include "imagine/math/theory/detail/ndarray/base.h"

namespace ig {

template <typename Xpr, typename Shp, typename Rng>
struct ndarray_traits
<
  view<Xpr, Shp, Rng>
>
{
  using value_type = ndarray_t<Xpr>;
};

template 
< typename x_,
  typename s_,
  typename r_ >
class view : public ndarray_base< view<x_, s_, r_> > {
public:
  explicit constexpr view(x_ xpr, const s_& shape, const r_& range)
    : xpr_{xpr}
    , shape_{shape}
    , range_{range}
    , strides_{}
    , size_{1}
    , off_ {0}
    , eff_ {0} {

    strides_[0] = 1;
    for (size_t i = 1; i < range_.dims(); ++i) strides_[i] = strides_[i - 1] * shape_[i - 1];
    std::transform(
      range_.rngs().begin(),
      range_.rngs().end(),
      shape_.begin(),
      shape_.begin(),
      [this, i = 0](auto& span, auto& dimension) mutable {
        auto  reduce = std::min(dimension, (span.end - span.begin + span.step - 1) / span.step);
        auto& stride = strides_[i++];
        
        size_  *= reduce;
        eff_   += (reduce * span.step != dimension);
        off_   += span.begin * stride;
        stride *= span.step;
        return reduce;
      });
  }

  auto size() const { return size_; }
  auto dims() const { return shape_.size(); }
  auto& shape() const   { return shape_; }
  auto& strides() const { return strides_; }

  template 
  < size_t... I, 
    typename... Ids >
  decltype(auto) impl(std::index_sequence<I...>, Ids... ids)
  { return xpr_((I < range_.dims() ? range_[I].begin + ids * range_[I].step : ids)...); }

  template <typename... Id>
  decltype(auto) operator()(Id... ids) const { return impl(std::index_sequence_for<Id...>{}, ids...); }
  template <typename... Id>
  decltype(auto) operator()(Id... ids)       { return impl(std::index_sequence_for<Id...>{}, ids...); }

  decltype(auto) eval(size_t id) const {
    return eval_strided(
      [this](auto i) { return i < eff_; },
      *this, 
       xpr_,
       strides_,
       id,
       off_);
  }

private:
  x_ xpr_;
  s_ shape_;
  r_ range_;
  std::array<size_t, r_::Ind> strides_;
  size_t size_;
  size_t off_, eff_;
};

// deduction
template 
< typename x_,
  typename s_,
  typename r_ > 
explicit view(x_ xpr, const s_& shape, const r_& range) 
      -> view< std::conditional_t<std::is_reference_v<x_>, x_, x_&>, s_, r_ >;

template 
< typename Cond, 
  typename Arr, 
  typename Rec,
  typename Strides >
decltype(auto) eval_strided(
  Cond&& cond, 
  Arr&& arr, 
  Rec&& rec, 
  Strides&& strides, size_t id, size_t begin = 0) {

  auto re = id;
  auto li = begin;
  size_t i = 0;
  for (; cond(i); ++i) {
    li += (re % arr[i]) * strides[i];
    re /= arr[i];
  }

  return 
    rec.eval(
      li +
      strides[i] * 
      re);
}

// Range
struct view_span 
{
  size_t begin = 0, end = -1, step = 1;
};

template <size_t Ind = 1> using view_span_r = std::array<view_span, Ind>;

template <typename Spn>
class view_ranges {
public:
  template <typename> 
  friend class view_ranges;

  static constexpr size_t Ind = std::tuple_size_v<Spn>;
  using uni = view_span_r<      1>;
  using inf = view_span_r<Ind - 1>;
  using sup = view_span_r<Ind + 1>;

  constexpr static auto all() { return view_ranges{}; }
  view_ranges() = default;
  explicit constexpr view_ranges(size_t begin)
    : ranges_{begin, begin + 1, 1} {}
  explicit constexpr view_ranges(
    size_t begin, 
    size_t end, 
    size_t step = 1)
    : ranges_{begin, end, step} {}
  explicit constexpr view_ranges(const view_ranges<inf>& lhs, const view_ranges<uni>& rhs)
    : ranges_{
      ranges_ind(lhs, rhs, std::make_index_sequence<Ind - 1>{})} {}

  constexpr auto  dims() const { return Ind; }
  constexpr auto& rngs() const { return ranges_; }
  constexpr auto& operator[](size_t i) const { return ranges_[i]; }

protected:
  template <size_t... N>
  constexpr auto ranges_ind(const view_ranges<inf>& lhs, const view_ranges<uni>& rhs, std::index_sequence<N...>) 
  { return std::array{lhs.ranges_[N]..., rhs.ranges_[0]}; }

  friend constexpr auto operator,(const view_ranges<Spn>& lhs, const view_ranges<uni>& rhs) { return view_ranges<sup>{lhs, rhs}; }
  Spn ranges_;
};

} // namespace ig

#endif // IG_MATH_NDARRAYVIEW_H
