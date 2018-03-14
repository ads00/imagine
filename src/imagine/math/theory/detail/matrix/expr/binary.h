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

#ifndef IG_MATH_MATRIXBINARY_H
#define IG_MATH_MATRIXBINARY_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template
< typename Lhs,
  typename Rhs,
  typename FunctionObject >
struct matrix_traits
<
  matrix_binary<Lhs, Rhs, FunctionObject>
>
{
  using value_type = std::common_type_t
    < matrix_t<Lhs>,
      matrix_t<Rhs>
    >; static constexpr auto n_rows = matrix_traits<Lhs>::n_rows, n_cols = matrix_traits<Lhs>::n_cols;
};

template
< typename l_,
  typename r_,
  typename f_ >
class matrix_binary : public matrix_base< matrix_binary<l_, r_, f_> > {
public:
  explicit matrix_binary(const l_& lhs, const r_& rhs, const f_& op)
    : lhs_{lhs}
    , rhs_{rhs}
    , op_{op} {}

  auto rows() const { return lhs_.rows(); }
  auto cols() const { return lhs_.cols(); }

  auto operator()(size_t row, size_t col) const
  { return op_(lhs_(row, col), rhs_(row, col)); }
  auto operator[](size_t n) const
  { return op_(lhs_[n], rhs_[n]); }

private:
  l_ lhs_;
  r_ rhs_;
  f_ op_;
};

template <typename Lhs, typename Rhs>
constexpr auto operator+(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix addition");
  return matrix_binary
    < Lhs,
      Rhs,
      std::plus<>
    >{lhs.derived(), rhs.derived(), std::plus<>{}};
}

template <typename Lhs, typename Rhs>
constexpr auto operator-(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix subtraction");
  return matrix_binary
    < Lhs,
      Rhs,
      std::minus<>
    >{lhs.derived(), rhs.derived(), std::minus<>{}};
}

template <typename Lhs, typename Rhs>
constexpr auto operator*(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix cwise multiplication");
  return matrix_binary
    < Lhs,
      Rhs,
      std::multiplies<>
    >{lhs.derived(), rhs.derived(), std::multiplies<>{}};
}

template <typename Lhs, typename Rhs>
constexpr auto operator/(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix division");
  return matrix_binary
    < Lhs,
      Rhs,
      std::divides<>
    >{lhs.derived(), rhs.derived(), std::divides<>{}};
}

template <typename Lhs, typename Rhs>
constexpr auto minima(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix cwise minima");

  auto minimum = [](auto lhs, auto rhs)
  { return std::min(lhs, rhs); };
  return matrix_binary
    < Lhs,
      Rhs,
      decltype(minimum)
    >{lhs.derived(), rhs.derived(), minimum};
}

template <typename Lhs, typename Rhs>
constexpr auto maxima(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix cwise maxima");

  auto maximum = [](auto lhs, auto rhs)
  { return std::max(lhs, rhs); };
  return matrix_binary
    < Lhs,
      Rhs,
      decltype(maximum)
    >{lhs.derived(), rhs.derived(), maximum};
}

// Scalars
template <typename S>
using scal_v =
  std::enable_if_t<
    std::is_scalar_v<S> ||
    (std::is_class_v<S> && std::is_trivially_copyable_v<S>)
  >;

template < typename Lhs, typename S, typename = scal_v<S> > constexpr auto operator+(const matrix_base<Lhs>& lhs, S rhs) { return u_expr(lhs, [rhs](auto& x) { return x + rhs; }); }
template < typename Rhs, typename S, typename = scal_v<S> > constexpr auto operator+(S lhs, const matrix_base<Rhs>& rhs) { return u_expr(rhs, [lhs](auto& x) { return lhs + x; }); }

template < typename Lhs, typename S, typename = scal_v<S> > constexpr auto operator-(const matrix_base<Lhs>& lhs, S rhs) { return u_expr(lhs, [rhs](auto& x) { return x - rhs; }); }
template < typename Rhs, typename S, typename = scal_v<S> > constexpr auto operator-(S lhs, const matrix_base<Rhs>& rhs) { return u_expr(rhs, [lhs](auto& x) { return lhs - x; }); }

template < typename Lhs, typename S, typename = scal_v<S> > constexpr auto operator*(const matrix_base<Lhs>& lhs, S rhs) { return u_expr(lhs, [rhs](auto& x) { return x * rhs; }); }
template < typename Rhs, typename S, typename = scal_v<S> > constexpr auto operator*(S lhs, const matrix_base<Rhs>& rhs) { return u_expr(rhs, [lhs](auto& x) { return lhs * x; }); }

template < typename Lhs, typename S, typename = scal_v<S> > constexpr auto operator/(const matrix_base<Lhs>& lhs, S rhs) { return u_expr(lhs, [rhs](auto& x) { return x / rhs; }); }
template < typename Rhs, typename S, typename = scal_v<S> > constexpr auto operator/(S lhs, const matrix_base<Rhs>& rhs) { return u_expr(rhs, [lhs](auto& x) { return lhs / x; }); }

} // namespace ig

#endif // IG_MATH_MATRIXBINARY_H
