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

#ifndef COMPUTE_LU_H
#define COMPUTE_LU_H

#include "imagine/compute/linalg/matrix.h"
#include "imagine/compute/linalg/solver.h"

namespace ig
{

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

template <typename TAlg>
class lu
{
public:
  using T = typename TAlg::T;
  using matrix_t = matrix<T>;
  using vector_t = vector<T>;

  static_assert(std::is_arithmetic<T>::value,
                "LU decomposition requires an arithmetic matrix");

  lu(const matrix_t& alg);

  T det() const;
  matrix_t inv() const;
  vector_t solve(const vector_t& b) const;

  constexpr const matrix_t& matrix() const       { return LU_; }
  constexpr const matrix_t& permutations() const { return P_; }

private:
  std::size_t permutations_;
  const std::size_t N_;

  matrix_t LU_;
  matrix_t P_;
};

template <typename TAlg>
lu<TAlg>::lu(const matrix_t& alg)
  : permutations_{0}, N_{alg.diagsize()}, LU_{alg}, P_{matrix_t::eye(N_)}
{
  for (std::size_t i = 0; i < N_; ++i)
  {
    // Find largest pivot element
    T piv = T(0); std::size_t row = i;
    for (std::size_t j = i; j < N_; ++j)
    {
      const T s = std::abs(LU_(j, i));
      if (s > piv) piv = s, row = j;
    }

    if (piv == T(0))
    {
      throw std::logic_error
        ("LU decomposition failed (Singular matrix)");
    }

    // Partial row pivoting
    if (row != i)
    {
      permutations_++;
      for (std::size_t j = 0; j < N_; ++j)
        std::swap(LU_(row, j), LU_(i, j)), std::swap(P_(row, j), P_(i, j));
    }

    // Factorize
    const T f = LU_(i, i);
    for (std::size_t j = i + 1; j < N_; ++j)
    {
      const T g = LU_(j, i) /= f;
      for (std::size_t k = i + 1; k < N_; ++k) LU_(j, k) -= g * LU_(i, k);
    }
  }
}

template <typename TAlg>
auto lu<TAlg>::det() const -> T
{
  int detsign = (permutations_ % 2) ? -1 : 1;
  return detsign * LU_.diag().prod();
}

template <typename TAlg>
auto lu<TAlg>::inv() const -> matrix_t
{
  matrix_t inverse{P_};
  for (std::size_t i = 0; i < N_; ++i)
  {
    auto invi = inverse.col(i);

    linalg::forward_solve (LU_, invi, true);
    linalg::backward_solve(LU_, invi);
  }

  return inverse;
}

template <typename TAlg>
auto lu<TAlg>::solve(const vector_t& b) const -> vector_t
{
  vector_t x{P_ * b};

  linalg::forward_solve (LU_, x, true);
  linalg::backward_solve(LU_, x);

  return x;
}

namespace linalg
{

template <typename TAlg>
lu<TAlg> lu_run(const alg<TAlg>& alg)
{
  assert(alg.square() && "LU decomposition requires a square matrix");
  return lu<TAlg>(alg);
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_LU_H
