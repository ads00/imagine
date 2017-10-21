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

#ifndef IG_MATH_MATRIXPRODUCT_H
#define IG_MATH_MATRIXPRODUCT_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template 
< typename Lhs, 
  typename Rhs >
struct matrix_traits
<
  matrix_product<Lhs, Rhs>
>
{
  using value_type = std::common_type_t
    < matrix_t<Lhs>,
      matrix_t<Rhs> 
    >;
  static constexpr auto D = matrix_traits<Lhs>::n_rows == dynamic_size || matrix_traits<Rhs>::n_cols == dynamic_size;
  static constexpr auto 
    n_rows = D ? dynamic_size : matrix_traits<Lhs>::n_rows,
    n_cols = D ? dynamic_size : matrix_traits<Rhs>::n_cols;
};

template 
< typename l_, 
  typename r_ >
class matrix_product : public matrix_base< matrix_product<l_, r_> > {
public:
  using matrix_type = concrete_matrix<matrix_product>;
  explicit matrix_product(const l_& lhs, const r_& rhs)
    : matrix_product{
        lhs,
        rhs, 
        std::integral_constant<bool, matrix_type::immutable>{}}
  { eval_product(lhs, rhs); }

  matrix_product(const l_& lhs, const r_& rhs, std::true_type)  : prod_{} {}
  matrix_product(const l_& lhs, const r_& rhs, std::false_type) : prod_{lhs.rows(), rhs.cols()} {}

  auto rows() const { return prod_.rows(); }
  auto cols() const { return prod_.cols(); }

  auto operator()(size_t row, size_t col) const 
  { return prod_(row, col); }
  auto operator[](size_t n) const 
  { return prod_[n]; }

private:
  void eval_product(const l_& lhs, const r_& rhs) {
    for (size_t i = 0; i < lhs.rows(); ++i)
      for (size_t j = 0; j < lhs.cols(); ++j)
        for (size_t k = 0; k < rhs.cols(); ++k) prod_(i, k) += lhs(i, j) * rhs(j, k);
  }

  matrix_type prod_;
};

template <typename Lhs, typename Rhs>
constexpr auto operator*(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.cols() == rhs.rows() && "Incoherent matrix-matrix multiplication");
  return matrix_product
    < Lhs, 
      Rhs
    >{lhs.derived(), rhs.derived()};
}

} // namespace ig

#endif // IG_MATH_MATRIXPRODUCT_H
