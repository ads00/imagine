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

#ifndef COMPUTE_SCALAR_EXPR_H
#define COMPUTE_SCALAR_EXPR_H

#include "imagine/compute/linalg/base/alg.h"
#include <functional>

namespace ig
{

template <typename TAlg, typename TOp>
struct alg_traits< scalar_expr<TAlg, TOp> >
{
  using T = alg_t<TAlg>;
  static constexpr int M = TAlg::M;
  static constexpr int N = TAlg::N;
};

template <typename TAlg, typename TOp>
class scalar_expr : public alg< scalar_expr<TAlg, TOp> >
{
public:
  scalar_expr(const TAlg& alg, const T scalar, const TOp& op)
    : alg_{alg}, scalar_{scalar}, op_{op} {}

  constexpr std::size_t rows() const { return alg_.rows(); }
  constexpr std::size_t cols() const { return alg_.cols(); }

  U operator()(std::size_t row, std::size_t col) const { return op_(alg_(row, col), scalar_); }
  U operator[](std::size_t n) const                    { return op_(alg_[n], scalar_); }

private:
  const TAlg alg_;
  const T scalar_;
  const TOp op_;
};

template <typename TAlg>
constexpr auto operator+(const alg<TAlg>& alg, alg_t<TAlg> scalar)
{
  return scalar_expr< TAlg, std::plus<> >{alg.derived(), scalar, std::plus<>{}};
}

template <typename TAlg>
constexpr auto operator-(const alg<TAlg>& alg, alg_t<TAlg> scalar)
{
  return scalar_expr< TAlg, std::minus<> >{alg.derived(), scalar, std::minus<>{}};
}

template <typename TAlg>
constexpr auto operator/(const alg<TAlg>& alg, alg_t<TAlg> scalar)
{
  return scalar_expr< TAlg, std::divides<> >{alg.derived(), scalar, std::divides<>{}};
}

template <typename TAlg>
constexpr auto operator/(alg_t<TAlg> scalar, const alg<TAlg>& alg)
{
  return scalar_expr< TAlg, std::divides<> >{alg.derived(), scalar, std::divides<>{}};
}

template <typename TAlg>
constexpr auto operator*(const alg<TAlg>& alg, alg_t<TAlg> scalar)
{
  return scalar_expr< TAlg, std::multiplies<> >{alg.derived(), scalar, std::multiplies<>{}};
}

template <typename TAlg>
constexpr auto operator*(alg_t<TAlg> scalar, const alg<TAlg>& alg)
{
  return scalar_expr< TAlg, std::multiplies<> >{alg.derived(), scalar, std::multiplies<>{}};
}

} // namespace ig

#endif // COMPUTE_SCALAR_EXPR_H
