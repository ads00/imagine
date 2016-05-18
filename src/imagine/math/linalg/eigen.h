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

#ifndef IG_MATH_EIGEN_H
#define IG_MATH_EIGEN_H

#include "imagine/math/linalg/matrix.h"

namespace ig {

/* 
 Compute an eigendecomposition of a N-by-N matrix A

 The factorization has the form
   A = VDV^-1

 - V is a column-orthogonal N-by-N matrix
 - D is a diagonal N-by-N matrix

 V is stored in a computed matrix and contains the eigenvectors as its columns
 D is stored in a computed matrix and contains the eigenvalues on its diagonal
*/

template <typename Alg, bool Sym = false> class eigen;

// Symmetric eigendecomposition
template <typename Alg>
class eigen<Alg, true> {
public:
  using T = alg_t<Alg>;
  using matrix_t = matrix<T>;
  using vector_t = vector<T>;

  static_assert(std::is_arithmetic<T>::value,
                "Eigendecomposition requires an arithmetic matrix");

  eigen(const matrix_t& alg);

  constexpr auto& eigenvectors() const { return V_; }
  constexpr auto& eigenvalues()  const { return D_; }

private:
  const size_t N_;

  matrix_t V_;
  vector_t D_;
};

template <typename Alg>
eigen<Alg, true>::eigen(const matrix_t& alg)
  : N_{alg.diagsize()}, V_{alg}, D_{N_} {

  auto work = vector_t{N_};
  // Symmetric Householder reduction to tridiagonal form
  for (size_t i = N_; i--> 1; ) {
    auto h = T(0);
    if (i - 1 > 0) {
      // Generate Householder vector
      for (size_t j = 0; j < i; ++j) h += V_(i, j) * V_(i, j);

      auto f = V_(i, i - 1);
      auto g = sign(f) * std::sqrt(h);

      work[i] = g;
      h -= f * g;

      V_(i, i - 1) = f - g;
      f = T(0);

      // Apply similarity transformation to remaining columns
      for (size_t j = 0; j < i; ++j) {
        V_(j, i) = V_(i, j) / h;

        g = T(0);
        for (size_t k = 0; k < j + 1; ++k) g += V_(j, k) * V_(i, k);
        for (size_t k = j + 1; k < i; ++k) g += V_(k, j) * V_(i, k);

        work[j] = g / h;
        f += work[j] * V_(i, j);
      }

      auto hh = f / (h + h);
      for (size_t j = 0; j < i; ++j) {
        f = V_(i, j);
        g = work[j] -= hh * f;
        for (size_t k = 0; k < j + 1; ++k) V_(j, k) -= f*work[k] + g*V_(i, k);
      }
    } else work[i] = V_(i, i - 1);

    D_[i] = h;
  }

  D_[0] = T(0), work[0] = T(0);

  // Accumulate transformations
  for (size_t i = 0; i < N_; ++i) {
    if (D_[i] != T(0)) {
      for (size_t j = 0; j < i; ++j) {
        auto g = T(0);
        for (size_t k = 0; k < i; ++k) g += V_(i, k) * V_(k, j);
        for (size_t k = 0; k < i; ++k) V_(k, j) -= g * V_(k, i);
      }
    }

    D_[i] = V_(i, i);
    V_(i, i) = T(1);
    for (size_t j = 0; j < i; ++j) V_(j, i) = V_(i, j) = T(0);
  }
  
  for (size_t i = 1; i < N_; ++i) work[i - 1] = work[i];

  // Symmetric tridiagonal QL algorithm
  auto sweeps = 40;
  for (size_t l = 0, m = 0; l < N_; ++l) {
    for (size_t it = 0; it <= sweeps; ++it) {
      // Find smallest subdiagonal element
      for (m = l; m < N_ - 1; ++m) {
        auto s = std::abs(D_[m]) + std::abs(D_[m + 1]);
        if (std::abs(work[m]) <= std::numeric_limits<T>::epsilon()/*eps<T>*/ * s)
          break;
      }

      // Check for convergence
      if (m == l) 
        break;
      if (it == sweeps) {
        throw std::logic_error("Eigendecomposition failed (No convergence)");
      }

      // Compute implicit shift
      auto d = (D_[l + 1] - D_[l]) / (T(2) * work[l]);
      auto g = (D_[m] - D_[l]) + work[l]/(d + sign(d)*std::hypot(d, T(1)));
      auto c = T(1), s = T(1), p = T(0);

      // Implicit QL transformation
      for (size_t i = m; i--> l; ) {
        auto b = c * work[i];
        auto f = s * work[i];
        auto r = work[i + 1] = std::hypot(f, g);

        if (r == T(0)) {
          D_[i + 1] -= p, work[m] = T(0);
          break;
        }

        s = f / r;
        c = g / r;
        g = D_[i + 1] - p;
        r = (D_[i] - g)*s + T(2)*c*b;
        D_[i + 1] = g + (p = s*r);
        g = c*r - b;

        // Accumulate transformation
        for (size_t k = 0; k < N_; ++k) {
          f = V_(k, i + 1);
          V_(k, i + 1) = s*V_(k, i) + c*f, V_(k, i) = c*V_(k, i) - s*f;
        }
      }

      D_[l] -= p;
      work[l] = g, work[m] = T(0);
    }
  }
}

namespace linalg {

template <typename Alg>
eigen<Alg, true> eig_sym_run(const alg<Alg>& alg) {
  assert(alg.square() && "Eigendecomposition requires a square matrix");
  return eigen<Alg, true>(alg);
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_EIGEN_H
