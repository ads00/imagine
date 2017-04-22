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

#include "imagine/math/theory/matrix.h"
#include "imagine/math/linalg/solver/direct.h"

namespace ig {

template <typename Mat>
class cholesky {
public:
  using type = mat_t<Mat>;
  using matrix_type = matrix<type>;
  using vector_type = colvec<type>;

  static_assert(std::is_arithmetic<type>::value, "Cholesky decomposition requires an arithmetic matrix");

  explicit cholesky(const matrix_type& mat);

  auto det() const -> type;
  auto inv() const -> matrix_type;
  auto solve(const vector_type& b) const -> vector_type;

  auto& mat() const { return llt_; }

private:
  const size_t n_;
  matrix_type llt_;
};

template <typename Mat>
cholesky<Mat>::cholesky(const matrix_type& mat)
  : n_{mat.diagsize()}
  , llt_{mat} {

  for (size_t i = 0; i < n_; ++i) {
    for (size_t j = i; j < n_; ++j) {
      auto s = llt_(i, j);
      for (size_t k = i; k--> 0; ) s -= llt_(i, k) * llt_(j, k);

      if (i == j) {
        // Diagonal square root
        if (s <= std::numeric_limits<type>::epsilon()) {
          throw std::logic_error{"Cholesky decomposition failed (Not positive-definite)"};
        }
        llt_(i, i) = std::sqrt(s);
      } else {
        llt_(j, i) = s / llt_(i, i);
        llt_(i, j) = 0;
      }
    }
  }
}

template <typename Mat>
auto cholesky<Mat>::det() const -> type {
  return std::pow(llt_.diag().prod(), 2);
}

template <typename Mat>
auto cholesky<Mat>::inv() const -> matrix_type {
  // Forward L-1
  auto inv = matrix_type::eye(n_);
  for (size_t i = 0; i < b_; ++i)
    lin::forward_solve(llt_, inv.col(i));
  return inv.t() * inv;
}

template <typename Mat>
auto cholesky<Mat>::solve(const vector_type& b) const -> vector_type {
  vector_type x{b};
  lin::forward_solve (llt_, x);
  lin::backward_solve(llt_.t(), x);
  return x;
}

namespace lin {

template <typename Mat>
constexpr auto chol_run(const matrix_base<Mat>& mat) {
  assert(mat.square() && "Cholesky decomposition requires a square matrix");
  return cholesky<Mat>{mat};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_CHOLESKY_H
