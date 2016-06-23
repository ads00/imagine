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

#ifndef IG_MATH_CHOLESKY_H
#define IG_MATH_CHOLESKY_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/equation.h"

namespace ig {

/* 
 Compute the Cholesky factorization of a real symmetric positive definite matrix A

 The factorization has the form
   A = L L^T

 - L is lower triangular
 - L^T is upper triangular

 L is stored in the lower triangular part, and its corresponding
 transposed L^T is stored in the upper part of the computer matrix
 The diagonal elements are commons to both matrix
*/

template <typename Alg>
class cholesky {
public:
  using T = alg_t<Alg>;
  using matrix_t = matrix<T>;
  using vector_t = vector<T>;

  static_assert(std::is_arithmetic<T>::value,
                "Cholesky decomposition requires an arithmetic matrix");

  cholesky(const matrix_t& mat);

  auto det() const -> T;
  auto inv() const -> matrix_t;
  auto solve(const vector_t& b) const -> vector_t;

  constexpr auto& matrix() const { return LLT_; }

private:
  const size_t N_;
  matrix_t LLT_;
};

template <typename Alg>
cholesky<Alg>::cholesky(const matrix_t& alg)
  : N_{alg.diagsize()}, LLT_{alg} {

  for (size_t i = 0; i < N_; ++i) {
    for (size_t j = i; j < N_; ++j) {
      auto s = LLT_(i, j);
      for (size_t k = i; k--> 0; ) s -= LLT_(i, k) * LLT_(j, k);

      if (i == j) {
        // Diagonal square root
        if (s <= std::numeric_limits<T>::epsilon()/*eps<T>*/) {
          throw std::logic_error{"Cholesky decomposition failed (Not positive-definite)"};
        }

        LLT_(i, i) = std::sqrt(s);
      } else {
        LLT_(j, i) = s / LLT_(i, i);
        LLT_(i, j) = T(0);
      }
    }
  }
}

template <typename Alg>
auto cholesky<Alg>::det() const -> T {
  return std::pow(LLT_.diag().prod(), 2);
}

template <typename Alg>
auto cholesky<Alg>::inv() const -> matrix_t {
  // Forward L-1
  auto inv = matrix_t::eye(N_);
  for (size_t i = 0; i < N_; ++i) {
    linalg::forward_solve(LLT_, inv.col(i));
  } return inv.t() * inv;
}

template <typename Alg>
auto cholesky<Alg>::solve(const vector_t& b) const -> vector_t {
  vector_t x{b};
  linalg::forward_solve (LLT_, x);
  linalg::backward_solve(LLT_.t(), x);
  return x;
}

namespace linalg {

template <typename Alg>
constexpr cholesky<Alg> chol_run(const alg<Alg>& alg) {
  assert(alg.square() && "Cholesky decomposition requires a square matrix");
  return cholesky<Alg>(alg);
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_CHOLESKY_H
