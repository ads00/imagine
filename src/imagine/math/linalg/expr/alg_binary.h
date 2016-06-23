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

#ifndef IG_MATH_BINARY_H
#define IG_MATH_BINARY_H

#include "imagine/math/linalg/base/alg.h"
#include <functional>

namespace ig {

template <typename Lhs, typename Rhs, typename Op>
struct alg_traits< binary_expr<Lhs, Rhs, Op> > {
  using T = std::common_type_t< alg_t<Lhs>, alg_t<Rhs> >;
  static constexpr auto M = Lhs::M, N = Lhs::N;
};

template <typename Lhs, typename Rhs, typename Op>
class binary_expr : public alg< binary_expr<Lhs, Rhs, Op> > {
public:
  constexpr binary_expr(const Lhs& lhs, const Rhs& rhs, const Op& op)
    : lhs_{lhs}, rhs_{rhs}, op_{op} {}

  constexpr auto rows() const { return lhs_.rows(); }
  constexpr auto cols() const { return lhs_.cols(); }

  auto operator()(size_t row, size_t col) const { return op_(lhs_(row, col), rhs_(row, col)); }
  auto operator[](size_t n) const               { return op_(lhs_[n], rhs_[n]); }

private:
  const Lhs lhs_;
  const Rhs rhs_;
  const Op op_;
};

template <typename Lhs, typename Rhs>
constexpr auto operator+(const alg<Lhs>& lhs, const alg<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix addition");
  return binary_expr< Lhs, Rhs, std::plus<> >{lhs.derived(), rhs.derived(), std::plus<>{}};
}

template <typename Lhs, typename Rhs>
constexpr auto operator-(const alg<Lhs>& lhs, const alg<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix subtraction");
  return binary_expr< Lhs, Rhs, std::minus<> >{lhs.derived(), rhs.derived(), std::minus<>{}};
}

template <typename Lhs, typename Rhs>
constexpr auto operator/(const alg<Lhs>& lhs, const alg<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix-matrix division");
  return binary_expr< Lhs, Rhs, std::divides<> >{lhs.derived(), rhs.derived(), std::divides<>{}};
}

template <typename Lhs, typename Rhs>
constexpr auto operator%(const alg<Lhs>& lhs, const alg<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix cwise multiplication");
  return binary_expr< Lhs, Rhs, std::multiplies<> >{lhs.derived(), rhs.derived(), std::multiplies<>{}};
}

} // namespace ig

#endif // IG_MATH_BINARY_H
