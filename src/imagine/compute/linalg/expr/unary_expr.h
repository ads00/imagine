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

#ifndef COMPUTE_UNARY_EXPR_H
#define COMPUTE_UNARY_EXPR_H

#include "imagine/compute/linalg/base/alg.h"
#include <functional>

namespace ig
{

template <typename TAlg, typename TOp>
struct alg_traits< unary_expr<TAlg, TOp> >
{
  using T = alg_t<TAlg>;
  static constexpr int M = TAlg::M;
  static constexpr int N = TAlg::N;
};

template <typename TAlg, typename TOp>
class unary_expr : public alg< unary_expr<TAlg, TOp> >
{
public:
  unary_expr(const TAlg& alg, const TOp& op)
    : alg_{alg}, op_{op} {}

  constexpr std::size_t rows() const { return alg_.rows(); }
  constexpr std::size_t cols() const { return alg_.cols(); }

  U operator()(std::size_t row, std::size_t col) const { return op_(alg_(row, col)); }
  U operator[](std::size_t n) const                    { return op_(alg_[n]); }

private:
  const TAlg alg_;
  const TOp op_;
};

template <typename TAlg>
constexpr auto operator-(const alg<TAlg>& alg)
{
  return unary_expr< TAlg, std::negate<> >{alg.derived(), std::negate<>{}};
}

namespace linalg
{

struct conj_op
{
  template <typename T>
  constexpr auto operator()(T&& val) const
  { return std::conj(std::forward<T>(val)); }
};

template <typename TAlg>
constexpr auto conj(const alg<TAlg>& alg)
{
  return unary_expr< TAlg, conj_op >{alg.derived(), conj_op{}};
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_UNARY_EXPR_H
