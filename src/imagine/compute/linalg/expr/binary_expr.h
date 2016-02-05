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

#ifndef COMPUTE_BINARY_EXPR_H
#define COMPUTE_BINARY_EXPR_H

#include "imagine/compute/linalg/base/alg.h"
#include <functional>

namespace ig
{

template <typename TLhs, typename TRhs, typename TOp>
struct alg_traits< binary_expr<TLhs, TRhs, TOp> >
{
  using T = std::common_type_t< alg_t<TLhs>, alg_t<TRhs> >;
  static constexpr int M = TLhs::M;
  static constexpr int N = TLhs::N;
};

template <typename TLhs, typename TRhs, typename TOp>
class binary_expr : public alg< binary_expr<TLhs, TRhs, TOp> >
{
public:
  binary_expr(const TLhs& lhs, const TRhs& rhs, const TOp& op)
    : lhs_{lhs}, rhs_{rhs}, op_{op} {}

  constexpr std::size_t rows() const { return lhs_.rows(); }
  constexpr std::size_t cols() const { return lhs_.cols(); }

  U operator()(std::size_t row, std::size_t col) const { return op_(lhs_(row, col), rhs_(row, col)); }
  U operator[](std::size_t n) const                    { return op_(lhs_[n], rhs_[n]); }

private:
  const TLhs lhs_;
  const TRhs rhs_;
  const TOp op_;
};

template <typename TLhs, typename TRhs>
constexpr auto operator+(const alg<TLhs>& lhs, const alg<TRhs>& rhs)
{
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix addition");
  return binary_expr< TLhs, TRhs, std::plus<> >{lhs.derived(), rhs.derived(), std::plus<>{}};
}

template <typename TLhs, typename TRhs>
constexpr auto operator-(const alg<TLhs>& lhs, const alg<TRhs>& rhs)
{
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix subtraction");
  return binary_expr< TLhs, TRhs, std::minus<> >{lhs.derived(), rhs.derived(), std::minus<>{}};
}

template <typename TLhs, typename TRhs>
constexpr auto operator/(const alg<TLhs>& lhs, const alg<TRhs>& rhs)
{
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix division");
  return binary_expr< TLhs, TRhs, std::divides<> >{lhs.derived(), rhs.derived(), std::divides<>{}};
}

template <typename TLhs, typename TRhs>
constexpr auto operator%(const alg<TLhs>& lhs, const alg<TRhs>& rhs)
{
  assert(lhs.size() == rhs.size() && "Incoherent matrix cwise multiplication");
  return binary_expr< TLhs, TRhs, std::multiplies<> >{lhs.derived(), rhs.derived(), std::multiplies<>{}};
}

} // namespace ig

#endif // COMPUTE_BINARY_EXPR_H
