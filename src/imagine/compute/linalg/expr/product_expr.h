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

#ifndef COMPUTE_PRODUCT_EXPR_H
#define COMPUTE_PRODUCT_EXPR_H

#include "imagine/compute/linalg/base/alg.h"
#include "imagine/compute/linalg/operation.h"

namespace ig
{

template <typename TLhs, typename TRhs>
struct alg_traits< product_expr<TLhs, TRhs> >
{
  using T = std::common_type_t< alg_t<TLhs>, alg_t<TRhs> >;
  static constexpr int M = TLhs::M;
  static constexpr int N = TRhs::N;
};

template <typename TLhs, typename TRhs>
class product_expr : public alg< product_expr<TLhs, TRhs> >
{
public:
  constexpr product_expr(const TLhs& lhs, const TRhs& rhs) 
    : lhs_{lhs}, rhs_{rhs} {}

  constexpr std::size_t rows() const { return lhs_.rows(); }
  constexpr std::size_t cols() const { return rhs_.cols(); }

  auto row_col_multiply(std::size_t row, std::size_t col) const
  {
    using T = typename alg_t< 
      std::remove_pointer_t<decltype(this)> >;

    T val = T(0);
    for (std::size_t i = 0; i < lhs_.cols(); ++i)
      val += lhs_(row, i) * rhs_(i, col);
    return val;
  }

  auto operator()(std::size_t row, std::size_t col) const { return row_col_multiply(row, col); }
  auto operator[](std::size_t n) const = delete;

private:
  const TLhs lhs_;
  const TRhs rhs_;
};

template <typename TLhs, typename TRhs>
constexpr auto operator*(const alg<TLhs>& lhs, const alg<TRhs>& rhs)
{
  assert(lhs.cols() == rhs.rows() && "Incoherent matrix-matrix multiplication");
  return product_expr<TLhs, TRhs>{lhs.derived(), rhs.derived()};
}

} // namespace ig

#endif // COMPUTE_PRODUCT_EXPR_H
