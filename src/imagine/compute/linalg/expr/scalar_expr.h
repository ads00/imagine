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

template <typename Alg, typename Op>
struct alg_traits< scalar_expr<Alg, Op> >
{
  using T = alg_t<Alg>;
  static constexpr int M = Alg::M;
  static constexpr int N = Alg::N;
};

template <typename Alg, typename Op>
class scalar_expr : public alg< scalar_expr<Alg, Op> >
{
public:
  constexpr scalar_expr(const Alg& alg, const T scalar, const Op& op)
    : alg_{alg}, scalar_{scalar}, op_{op} {}

  constexpr std::size_t rows() const { return alg_.rows(); }
  constexpr std::size_t cols() const { return alg_.cols(); }

  auto operator()(std::size_t row, std::size_t col) const { return op_(alg_(row, col), scalar_); }
  auto operator[](std::size_t n) const                    { return op_(alg_[n], scalar_); }

private:
  const Alg alg_;
  const T scalar_;
  const Op op_;
};

template <typename Alg>
constexpr auto operator+(const alg<Alg>& alg, alg_t<Alg> scalar)
{
  return scalar_expr< Alg, std::plus<> >{alg.derived(), scalar, std::plus<>{}};
}

template <typename Alg>
constexpr auto operator-(const alg<Alg>& alg, alg_t<Alg> scalar)
{
  return scalar_expr< Alg, std::minus<> >{alg.derived(), scalar, std::minus<>{}};
}

template <typename Alg>
constexpr auto operator/(const alg<Alg>& alg, alg_t<Alg> scalar)
{
  return scalar_expr< Alg, std::divides<> >{alg.derived(), scalar, std::divides<>{}};
}

template <typename Alg>
constexpr auto operator/(alg_t<Alg> scalar, const alg<Alg>& alg)
{
  return scalar_expr< Alg, std::divides<> >{alg.derived(), scalar, std::divides<>{}};
}

template <typename Alg>
constexpr auto operator*(const alg<Alg>& alg, alg_t<Alg> scalar)
{
  return scalar_expr< Alg, std::multiplies<> >{alg.derived(), scalar, std::multiplies<>{}};
}

template <typename Alg>
constexpr auto operator*(alg_t<Alg> scalar, const alg<Alg>& alg)
{
  return scalar_expr< Alg, std::multiplies<> >{alg.derived(), scalar, std::multiplies<>{}};
}

} // namespace ig

#endif // COMPUTE_SCALAR_EXPR_H
