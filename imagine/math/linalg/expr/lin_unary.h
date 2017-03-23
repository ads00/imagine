/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef IG_MATH_UNARY_H
#define IG_MATH_UNARY_H

#include "imagine/math/linalg/base/lin.h"
#include <functional>

namespace ig {

template <typename Lin, typename Op>
struct lin_traits< unary_expr<Lin, Op> > {
  using T = lin_t<Lin>;
  static constexpr auto M = Lin::M, N = Lin::N;
};

template <typename Fn>
struct unary_fn {
  template <typename T> constexpr auto operator()(const T& val) const { return fn_(val); }
  Fn fn_;
};

template <typename Lin, typename Op>
class unary_expr : public lin_base< unary_expr<Lin, Op> > {
public:
  explicit unary_expr(const Lin& lin, const Op& op)
    : lin_{lin}
    , op_{op} {}

  auto rows() const { return lin_.rows(); }
  auto cols() const { return lin_.cols(); }

  auto operator()(size_t row, size_t col) const { return op_(lin_(row, col)); }
  auto operator[](size_t n) const               { return op_(lin_[n]); }

private:
  const Lin lin_;
  const Op op_;
};

template <typename Lin>
constexpr auto operator-(const lin_base<Lin>& lin) {
  return unary_expr< Lin, std::negate<> >{lin.derived(), std::negate<>{}};
}

// Unary lambda-based function applier
template <typename Lin, typename Fn>
constexpr auto apply_fn(const lin_base<Lin>& lin, Fn&& fn) 
{ return unary_expr< Lin, unary_fn<Fn> >{lin.derived(), unary_fn<Fn>{fn}}; }

template <typename Lin>
constexpr auto operator+(const lin_base<Lin>& lin, lin_t<Lin> scalar) 
{ return apply_fn(lin, [scalar](auto& val) { return val + scalar; }); }

template <typename Lin>
constexpr auto operator-(const lin_base<Lin>& lin, lin_t<Lin> scalar) 
{ return apply_fn(lin, [scalar](auto& val) { return val - scalar; }); }

template <typename Lin>
constexpr auto operator-(lin_t<Lin> scalar, const lin_base<Lin>& lin) 
{ return apply_fn(lin, [scalar](auto& val) { return scalar - val; }); }

template <typename Lin>
constexpr auto operator/(const lin_base<Lin>& lin, lin_t<Lin> scalar) 
{ return apply_fn(lin, [scalar](auto& val) { return val / scalar; }); }

template <typename Lin>
constexpr auto operator/(lin_t<Lin> scalar, const lin_base<Lin>& lin) 
{ return apply_fn(lin, [scalar](auto& val) { return scalar / val; }); }

template <typename Lin>
constexpr auto operator*(const lin_base<Lin>& lin, lin_t<Lin> scalar) 
{ return apply_fn(lin, [scalar](auto& val) { return val * scalar; }); }

template <typename Lin>
constexpr auto operator*(lin_t<Lin> scalar, const lin_base<Lin>& lin) 
{ return apply_fn(lin, [scalar](auto& val) { return scalar * val; }); }

} // namespace ig

#endif // IG_MATH_UNARY_H
