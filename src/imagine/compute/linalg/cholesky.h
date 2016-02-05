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

#ifndef COMPUTE_CHOLESKY_H
#define COMPUTE_CHOLESKY_H

#include "imagine/compute/linalg/matrix.h"
#include "imagine/compute/linalg/solver.h"

namespace ig
{

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

template <typename TAlg>
class cholesky
{
public:
  using T = typename TAlg::T;
  using matrix_t = matrix<T>;
  using vector_t = vector<T>;

  static_assert(std::is_arithmetic<T>::value,
                "Cholesky decomposition requires an arithmetic matrix");

  cholesky(const matrix_t& mat);

  T det() const;
  matrix_t inv() const;
  vector_t solve(const vector_t& b) const;

  constexpr const matrix_t& matrix() const { return LLT_; }

private:
  const std::size_t N_;
  matrix_t LLT_;
};

template <typename TAlg>
cholesky<TAlg>::cholesky(const matrix_t& alg)
  : N_{alg.diagsize()}, LLT_{alg}
{
  for (std::size_t i = 0; i < N_; ++i)
  {
    for (std::size_t j = i; j < N_; ++j)
    {
      T s = LLT_(i, j);
      for (std::size_t k = i; k--> 0; ) s -= LLT_(i, k) * LLT_(j, k);

      if (i == j)
      {
        // Diagonal square root
        if (s <= eps<T>())
        {
          std::logic_error exc("Cholesky decomposition failed (Not positive-definite)");
          std::throw_with_nested(exc);
        }

        LLT_(i, i) = std::sqrt(s);
      }
      else
      {
        LLT_(j, i) = s / LLT_(i, i);
        LLT_(i, j) = T(0);
      }
    }
  }
}

template <typename TAlg>
auto cholesky<TAlg>::det() const -> T
{
  return std::pow(LLT_.diag().prod(), 2);
}

template <typename TAlg>
auto cholesky<TAlg>::inv() const -> matrix_t
{
  // Forward L-1
  matrix_t inverse{matrix_t::eye(N_)};
  for (std::size_t i = 0; i < N_; ++i)
  {
    auto invi = inverse.col(i);
    linalg::forward_solve(LLT_, invi);
  }

  // Compute inv = L-1T * L-1
  return inverse.t() * inverse;
}

template <typename TAlg>
auto cholesky<TAlg>::solve(const vector_t& b) const -> vector_t
{
  vector_t x{b};

  linalg::forward_solve (LLT_, x);
  linalg::backward_solve(LLT_.t(), x);

  return x;
}

namespace linalg
{

template <typename TAlg>
constexpr cholesky<TAlg> chol_run(const alg<TAlg>& alg)
{
  assert(alg.square() && "Cholesky decomposition requires a square matrix");
  return cholesky<TAlg>(alg);
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_CHOLESKY_H
