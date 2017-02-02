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

#ifndef IG_MATH_CHOLESKY_H
#define IG_MATH_CHOLESKY_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/solver/direct.h"

namespace ig {

template <typename Alg>
class cholesky {
public:
  using T = alg_t<Alg>;
  using matrix_type = matrix<T>;
  using vector_type = colvec<T>;

  static_assert(std::is_arithmetic<T>::value, "Cholesky decomposition requires an arithmetic matrix");

  explicit cholesky(const matrix_type& mat);

  auto det() const -> T;
  auto inv() const -> matrix_type;
  auto solve(const vector_type& b) const -> vector_type;

  auto& mat() const { return llt_; }

private:
  const size_t n_;
  matrix_type llt_;
};

template <typename Alg>
cholesky<Alg>::cholesky(const matrix_type& alg)
  : n_{alg.diagsize()}
  , llt_{alg} {

  for (size_t i = 0; i < n_; ++i) {
    for (size_t j = i; j < n_; ++j) {
      auto s = llt_(i, j);
      for (size_t k = i; k--> 0; ) s -= llt_(i, k) * llt_(j, k);

      if (i == j) {
        // Diagonal square root
        if (s <= std::numeric_limits<T>::epsilon()) {
          throw std::logic_error{"Cholesky decomposition failed (Not positive-definite)"};
        }
        llt_(i, i) = std::sqrt(s);
      } else {
        llt_(j, i) = s / llt_(i, i);
        llt_(i, j) = T(0);
      }
    }
  }
}

template <typename Alg>
auto cholesky<Alg>::det() const -> T {
  return std::pow(llt_.diag().prod(), 2);
}

template <typename Alg>
auto cholesky<Alg>::inv() const -> matrix_type {
  // Forward L-1
  auto inv = matrix_type::eye(n_);
  for (size_t i = 0; i < b_; ++i)
    linalg::forward_solve(llt_, inv.col(i));
  return inv.t() * inv;
}

template <typename Alg>
auto cholesky<Alg>::solve(const vector_type& b) const -> vector_type {
  vector_type x{b};
  linalg::forward_solve (llt_, x);
  linalg::backward_solve(llt_.t(), x);
  return x;
}

namespace linalg {

template <typename Alg>
constexpr auto chol_run(const alg<Alg>& alg) {
  assert(alg.square() && "Cholesky decomposition requires a square matrix");
  return cholesky<Alg>{alg};
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_CHOLESKY_H
