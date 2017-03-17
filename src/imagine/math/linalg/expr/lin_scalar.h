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

#ifndef IG_MATH_SCALAR_H
#define IG_MATH_SCALAR_H

#include "imagine/math/linalg/base/lin.h"
#include <functional>

namespace ig {

template <typename Lin, typename Op>
struct lin_traits< scalar_expr<Lin, Op> > {
  using T = lin_t<Lin>;
  static constexpr auto M = Lin::M, N = Lin::N;
};

template <typename Lin, typename Op>
class scalar_expr : public lin_base< scalar_expr<Lin, Op> > {
public:
  using T = lin_t<scalar_expr>;

  explicit scalar_expr(const Lin& lin, const T scalar, const Op& op)
    : lin_{lin}
    , scalar_{scalar}
    , op_{op} {}

  auto rows() const { return lin_.rows(); }
  auto cols() const { return lin_.cols(); }

  auto operator()(size_t row, size_t col) const { return op_(lin_(row, col), scalar_); }
  auto operator[](size_t n) const               { return op_(lin_[n], scalar_); }

private:
  const Lin lin_;
  const T scalar_;
  const Op op_;
};

template <typename Lin>
constexpr auto operator+(const lin_base<Lin>& lin, lin_t<Lin> scalar) {
  return scalar_expr< Lin, std::plus<> >{lin.derived(), scalar, std::plus<>{}};
}

template <typename Lin>
constexpr auto operator-(const lin_base<Lin>& lin, lin_t<Lin> scalar) {
  return scalar_expr< Lin, std::minus<> >{lin.derived(), scalar, std::minus<>{}};
}

template <typename Lin>
constexpr auto operator/(const lin_base<Lin>& lin, lin_t<Lin> scalar) {
  return scalar_expr< Lin, std::divides<> >{lin.derived(), scalar, std::divides<>{}};
}

template <typename Lin>
constexpr auto operator/(lin_t<Lin> scalar, const lin_base<Lin>& lin) {
  return scalar_expr< Lin, std::divides<> >{lin.derived(), scalar, std::divides<>{}};
}

template <typename Lin>
constexpr auto operator*(const lin_base<Lin>& lin, lin_t<Lin> scalar) {
  return scalar_expr< Lin, std::multiplies<> >{lin.derived(), scalar, std::multiplies<>{}};
}

template <typename Lin>
constexpr auto operator*(lin_t<Lin> scalar, const lin_base<Lin>& lin) {
  return scalar_expr< Lin, std::multiplies<> >{lin.derived(), scalar, std::multiplies<>{}};
}

} // namespace ig

#endif // IG_MATH_SCALAR_H
