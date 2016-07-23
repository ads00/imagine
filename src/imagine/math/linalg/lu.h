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

#ifndef IG_MATH_LU_H
#define IG_MATH_LU_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/solver/direct.h"

namespace ig {

/* 
 Compute a LU factorization of a general N-by-N matrix A
 using partial pivoting with row interchange

 The factorization has the form
   P A = L U

 - P is a permutation matrix
 - L is unit lower triangular
 - U is upper triangular

 P is stored in a separate permutation matrix
 L is stored in the strict lower triangular part of the
 computed matrix. The diagonal elements are unity and are not stored
 U is stored in the diagonal and upper triangular part of the computed matrix
*/

template <typename Alg>
class lu {
public:
  using T = alg_t<Alg>;
  using matrix_type = matrix<T>;
  using vector_type = colvec<T>;

  static_assert(std::is_arithmetic<T>::value, "LU decomposition requires an arithmetic matrix");

  explicit lu(const matrix_type& alg);

  auto det() const -> T;
  auto inv() const -> matrix_type;
  auto solve(const vector_type& b) const -> vector_type;

  auto& mat() const   { return LU_; }
  auto& perms() const { return P_; }

private:
  const size_t N_;
  size_t permutations_;

  matrix_type LU_;
  matrix_type P_;
};

template <typename Alg>
lu<Alg>::lu(const matrix_type& alg)
  : N_{alg.diagsize()}, permutations_{0}, LU_{alg}, P_{matrix_type::eye(N_)} {

  for (size_t i = 0, row = i; i < N_; ++i) {
    // Find largest pivot element
    auto pivot = T(0);
    for (size_t j = i; j < N_; ++j) {
      auto curr_elemt = std::abs(LU_(j, i)); 
      if (curr_elemt > pivot) pivot = curr_elemt, row = j;
    }

    if (pivot == T(0)) {
      throw std::logic_error{"LU decomposition failed (Singular matrix)"};
    }

    // Partial row pivoting
    if (row != i) {
      permutations_++;
      for (size_t j = 0; j < N_; ++j) {
        std::swap(LU_(row, j), LU_(i, j)), std::swap(P_(row, j), P_(i, j));
      }
    }

    // Factorize
    auto f = LU_(i, i);
    for (size_t j = i + 1; j < N_; ++j) {
      auto g = LU_(j, i) /= f;
      for (size_t k = i + 1; k < N_; ++k) LU_(j, k) -= g * LU_(i, k);
    }
  }
}

template <typename Alg>
auto lu<Alg>::det() const -> T {
  auto detsign = (permutations_ % 2) ? 
    -1 :
     1;
  return detsign * LU_.diag().prod();
}

template <typename Alg>
auto lu<Alg>::inv() const -> matrix_type {
  // Solve for each column on eye matrix
  auto inv = P_;
  for (size_t i = 0; i < N_; ++i) {
    linalg::forward_solve (LU_, inv.col(i), true);
    linalg::backward_solve(LU_, inv.col(i));
  } return inv;
}

template <typename Alg>
auto lu<Alg>::solve(const vector_type& b) const -> vector_type {
  vector_type x{P_ * b};
  linalg::forward_solve (LU_, x, true);
  linalg::backward_solve(LU_, x);
  return x;
}

namespace linalg {

template <typename Alg>
constexpr auto lu_run(const alg<Alg>& alg) {
  assert(alg.square() && "LU decomposition requires a square matrix");
  return lu<Alg>{alg};
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_LU_H
