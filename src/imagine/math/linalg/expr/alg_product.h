/*
 Copyright (c) 2015, 2016
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

#include "imagine/math/linalg/base/alg.h"
#include "imagine/math/linalg/operation.h"

namespace ig {

template <typename Lhs, typename Rhs>
struct alg_traits< product_expr<Lhs, Rhs> > {
  using T = std::common_type_t< alg_t<Lhs>, alg_t<Rhs> >;
  static constexpr auto M = Lhs::M, N = Rhs::N;
};

template <typename Lhs, typename Rhs>
class product_expr : public alg< product_expr<Lhs, Rhs> > {
public:
  using matrix_type = typename product_expr::plain_type;

  explicit product_expr(const Lhs& lhs, const Rhs& rhs)
    : product_expr{lhs, rhs, 
                   std::integral_constant<bool, matrix_type::immutable>{},
                   std::integral_constant<bool, matrix_type::hybrid>{}}
  { eval_product(lhs, rhs); }

  product_expr(const Lhs& lhs, const Rhs& rhs, std::true_type,  std::false_type) : prod_{} {}
  product_expr(const Lhs& lhs, const Rhs& rhs, std::false_type, std::false_type) : prod_{lhs.rows(), rhs.cols()} {}
  product_expr(const Lhs& lhs, const Rhs& rhs, std::false_type, std::true_type)  : prod_{matrix_type::dynamic_rows ? lhs.rows() : rhs.cols()} {}

  auto rows() const { return prod_.rows(); }
  auto cols() const { return prod_.cols(); }

  auto operator()(size_t row, size_t col) const { return prod_(row, col); }
  auto operator[](size_t n) const               { return prod_[n]; }

private:
  void eval_product(const Lhs& lhs, const Rhs& rhs) {
    typename Lhs::plain_type ev_lhs = lhs;
    typename Rhs::plain_type ev_rhs = rhs;

    for (size_t i = 0; i < lhs.rows(); ++i)
      for (size_t j = 0; j < rhs.cols(); ++j)
        for (size_t k = 0; k < lhs.cols(); ++k) prod_(i, j) += ev_lhs(i, k) * ev_rhs(k, j);
  }

  matrix_type prod_;
};

template <typename Lhs, typename Rhs>
constexpr auto operator*(const alg<Lhs>& lhs, const alg<Rhs>& rhs) {
  assert(lhs.cols() == rhs.rows() && "Incoherent matrix-matrix multiplication");
  return product_expr<Lhs, Rhs>{lhs.derived(), rhs.derived()};
}

} // namespace ig

#endif // IG_MATH_PRODUCT_H
