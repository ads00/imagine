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

#ifndef IG_MATH_PRODUCT_EXPR_H
#define IG_MATH_PRODUCT_EXPR_H

#include "imagine/math/linalg/base/alg.h"
#include "imagine/math/linalg/operation.h"

namespace ig {

template <typename Lhs, typename Rhs>
struct alg_traits< product_expr<Lhs, Rhs> > {

  using T = std::common_type_t< alg_t<Lhs>, alg_t<Rhs> >;
  static constexpr auto M = Lhs::M;
  static constexpr auto N = Rhs::N;
};

template <typename Lhs, typename Rhs>
class product_expr : public alg< product_expr<Lhs, Rhs> > {
public:
  constexpr product_expr(const Lhs& lhs, const Rhs& rhs) 
    : lhs_{lhs}, rhs_{rhs} {}

  constexpr auto rows() const { return lhs_.rows(); }
  constexpr auto cols() const { return rhs_.cols(); }

  auto row_col_multiply(size_t row, size_t col) const {
    using T = typename alg_t< 
      std::remove_pointer_t<decltype(this)> >;

    auto val = T(0);
    for (size_t i = 0; i < lhs_.cols(); ++i) {
      val += lhs_(row, i) * rhs_(i, col);
    }
    return val;
  }

  auto operator()(size_t row, size_t col) const { return row_col_multiply(row, col); }
  auto operator[](size_t n) const = delete;

private:
  const Lhs lhs_;
  const Rhs rhs_;
};

template <typename Lhs, typename Rhs>
constexpr auto operator*(const alg<Lhs>& lhs, const alg<Rhs>& rhs) {
  assert(lhs.cols() == rhs.rows() && "Incoherent matrix-matrix multiplication");
  return product_expr<Lhs, Rhs>{lhs.derived(), rhs.derived()};
}

} // namespace ig

#endif // IG_MATH_PRODUCT_EXPR_H
