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

#ifndef IG_MATH_PRODUCT_H
#define IG_MATH_PRODUCT_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename Lhs, typename Rhs>
struct mat_traits
<
  product_expr<Lhs, Rhs>
>
{
  using type = std::common_type_t
    < mat_t<Lhs>, 
      mat_t<Rhs> 
    >;
  static constexpr auto D = 
    mat_traits<Lhs>::n_rows == dynamic_sized ||
    mat_traits<Rhs>::n_cols == dynamic_sized;
  static constexpr auto n_rows = D ? -1 : mat_traits<Lhs>::n_rows, 
                        n_cols = D ? -1 : mat_traits<Rhs>::n_cols;
};

template <typename l_, typename r_>
class product_expr : public matrix_base< product_expr<l_, r_> > {
public:
  using matrix_type = concrete_mat_t<product_expr>;
  explicit product_expr(const l_& lhs, const r_& rhs)
    : product_expr{lhs, 
                   rhs, 
                   std::integral_constant<bool, matrix_type::immutable>{}}
  { eval_product(lhs, rhs); }

  product_expr(const l_& lhs, const r_& rhs, std::true_type)  : prod_{} {}
  product_expr(const l_& lhs, const r_& rhs, std::false_type) : prod_{lhs.rows(), rhs.cols()} {}

  auto rows() const { return prod_.rows(); }
  auto cols() const { return prod_.cols(); }

  auto operator()(size_t row, size_t col) const { return prod_(row, col); }
  auto operator[](size_t n) const               { return prod_[n]; }

private:
  void eval_product(const l_& lhs, const r_& rhs) {
    for (size_t i = 0; i < lhs.cols(); ++i)
      for (size_t j = 0; j < rhs.cols(); ++j)
        for (size_t k = 0; k < lhs.rows(); ++k) prod_(k, j) += lhs(k, i) * rhs(i, j);
  }

  matrix_type prod_;
};

template <typename Lhs, typename Rhs>
constexpr auto operator*(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.cols() == rhs.rows() && "Incoherent matrix-matrix multiplication");
  return product_expr
    < Lhs, 
      Rhs
    >{lhs.derived(), rhs.derived()};
}

} // namespace ig

#endif // IG_MATH_PRODUCT_H
