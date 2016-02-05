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

#ifndef COMPUTE_EIGEN_H
#define COMPUTE_EIGEN_H

#include "imagine/compute/linalg/matrix.h"

namespace ig
{

/* 
 Compute an eigendecomposition of a N-by-N matrix A

 The factorization has the form
   A = VDV^-1

 - V is a column-orthogonal N-by-N matrix
 - D is a diagonal N-by-N matrix

 V is stored in a computed matrix and contains the eigenvectors as its columns

 D is stored in a computed matrix and contains the eigenvalues on its diagonal
*/

template <typename TAlg, bool Sym = false> class eigen;

// Symmetric eigendecomposition
template <typename TAlg>
class eigen<TAlg, true>
{
public:
  using T = typename TAlg::T;
  using matrix_t = matrix<T>;
  using vector_t = vector<T>;

  static_assert(std::is_arithmetic<T>::value,
                "Eigendecomposition requires an arithmetic matrix");

  eigen(const matrix_t& alg);

  constexpr const matrix_t& eigenvectors() const { return V_; }
  constexpr const vector_t& eigenvalues()  const { return D_; }

private:
  const std::size_t N_;

  matrix_t V_;
  vector_t D_;
};

template <typename TAlg>
eigen<TAlg, true>::eigen(const matrix_t& alg)
  : N_{alg.diagsize()}, V_{alg}, D_{N_}
{
  vector_t work{N_};

  // Symmetric Householder reduction to tridiagonal form
  for (std::size_t i = N_; i--> 1; )
  {
    T h = T(0);
    if (i - 1 > 0)
    {
      // Generate Householder vector
      for (std::size_t j = 0; j < i; ++j) h += V_(i, j) * V_(i, j);

      T f = V_(i, i - 1);
      T g = sign(f) * std::sqrt(h);

      work[i] = g;
      h -= f * g;

      V_(i, i - 1) = f - g;
      f = T(0);

      // Apply similarity transformation to remaining columns
      for (std::size_t j = 0; j < i; ++j)
      {
        V_(j, i) = V_(i, j) / h;

        g = T(0);
        for (std::size_t k = 0; k < j + 1; ++k) g += V_(j, k) * V_(i, k);
        for (std::size_t k = j + 1; k < i; ++k) g += V_(k, j) * V_(i, k);

        work[j] = g / h;
        f += work[j] * V_(i, j);
      }

      T hh = f / (h + h);
      for (std::size_t j = 0; j < i; ++j)
      {
        f = V_(i, j);
        g = work[j] -= hh * f;
        for (std::size_t k = 0; k < j + 1; ++k) V_(j, k) -= f*work[k] + g*V_(i, k);
      }
    }
    else work[i] = V_(i, i - 1);

    D_[i] = h;
  }

  D_[0] = T(0);
  work[0] = T(0);

  // Accumulate transformations
  for (std::size_t i = 0; i < N_; ++i)
  {
    if (D_[i] != T(0))
    {
      for (std::size_t j = 0; j < i; ++j)
      {
        T g = T(0);
        for (std::size_t k = 0; k < i; ++k) g += V_(i, k) * V_(k, j);
        for (std::size_t k = 0; k < i; ++k) V_(k, j) -= g * V_(k, i);
      }
    }

    D_[i] = V_(i, i);
    V_(i, i) = T(1);
    for (std::size_t j = 0; j < i; ++j) V_(j, i) = V_(i, j) = T(0);
  }
  
  for (std::size_t i = 1; i < N_; ++i) work[i - 1] = work[i];

  // Symmetric tridiagonal QL algorithm
  const std::size_t sweeps = 40;
  for (std::size_t l = 0; l < N_; ++l)
  {
    std::size_t m = 0;
    for (std::size_t it = 0; it <= sweeps; ++it)
    {
      // Find smallest subdiagonal element
      for (m = l; m < N_ - 1; ++m)
      {
        const T ab = std::abs(D_[m]) + std::abs(D_[m + 1]);
        if (std::abs(work[m]) <= eps<T>() * ab)
          break;
      }

      // Check for convergence
      if (m == l) break;
      if (it == sweeps)
      {
        std::logic_error exc("Eigendecomposition failed (No convergence)");
        std::throw_with_nested(exc);
      }

      // Compute implicit shift
      T g = (D_[l + 1] - D_[l]) / (T(2) * work[l]);
      g = (D_[m] - D_[l]) + work[l]/(g + sign(g)*std::hypot(g, T(1)));

      T c = T(1);
      T s = T(1);
      T p = T(0);

      // Implicit QL transformation
      for (std::size_t i = m; i--> l; )
      {
        const T b = c * work[i];

        T f = s * work[i];
        T r = work[i + 1] = std::hypot(f, g);
        if (r == T(0))
        {
          D_[i + 1] -= p;
          work[m] = T(0);
          break;
        }

        s = f / r;
        c = g / r;
        g = D_[i + 1] - p;
        r = (D_[i] - g)*s + T(2)*c*b;
        D_[i + 1] = g + (p = s*r);
        g = c*r - b;

        // Accumulate transformation
        for (std::size_t k = 0; k < N_; ++k)
        {
          f = V_(k, i + 1);
          V_(k, i + 1) = s*V_(k, i) + c*f;
          V_(k, i) = c*V_(k, i) - s*f;
        }
      }

      D_[l] -= p;
      work[l] = g;
      work[m] = T(0);
    }
  }
}

namespace linalg
{

template <typename TAlg>
eigen<TAlg, true> eig_sym_run(const alg<TAlg>& alg)
{
  assert(alg.square() && "Eigendecomposition requires a square matrix");
  return eigen<TAlg, true>(alg);
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_EIGEN_H
