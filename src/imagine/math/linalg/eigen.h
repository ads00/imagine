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

#ifndef IG_MATH_EIGEN_H
#define IG_MATH_EIGEN_H

#include "imagine/math/linalg/matrix.h"

namespace ig {

template <typename Lin, bool Sym = false> class eigen;

// Symmetric eigendecomposition
template <typename Lin>
class eigen<Lin, true> {
public:
  using T = lin_t<Lin>;
  using matrix_type = matrix<T>;
  using vector_type = colvec<T>;

  static_assert(std::is_arithmetic<T>::value, "Eigendecomposition requires an arithmetic matrix");

  explicit eigen(const matrix_type& lin);

  auto& eigenvectors() const { return v_; }
  auto& eigenvalues() const  { return d_; }

private:
  const size_t n_;

  matrix_type v_;
  vector_type d_;
};

template <typename Lin>
eigen<Lin, true>::eigen(const matrix_type& lin)
  : n_{lin.diagsize()}
  , v_{lin}
  , d_{n_} {

  vector_type work{n_};
  // Symmetric Householder reduction to tridiagonal form
  for (size_t i = n_; i--> 1; ) {
    auto h = T(0);
    if (i - 1 > 0) {
      // Generate Householder vector
      for (size_t j = 0; j < i; ++j) h += v_(i, j) * v_(i, j);

      auto f = v_(i, i - 1);
      auto g = sign(f) * std::sqrt(h);

      work[i] = g;
      h -= f * g;

      v_(i, i - 1) = f - g;
      f = T(0);

      // Apply similarity transformation to remaining columns
      for (size_t j = 0; j < i; ++j) {
        v_(j, i) = v_(i, j) / h;

        g = T(0);
        for (size_t k = 0; k < j + 1; ++k) g += v_(j, k) * v_(i, k);
        for (size_t k = j + 1; k < i; ++k) g += v_(k, j) * v_(i, k);

        work[j] = g / h;
        f += work[j] * v_(i, j);
      }

      auto hh = f / (h + h);
      for (size_t j = 0; j < i; ++j) {
        f = v_(i, j);
        g = work[j] -= hh * f;
        for (size_t k = 0; k < j + 1; ++k) v_(j, k) -= f * work[k] + g * v_(i, k);
      }
    } else work[i] = v_(i, i - 1);

    d_[i] = h;
  }

  d_[0] = T(0), work[0] = T(0);

  // Accumulate transformations
  for (size_t i = 0; i < n_; ++i) {
    if (d_[i] != T(0)) {
      for (size_t j = 0; j < i; ++j) {
        auto g = T(0);
        for (size_t k = 0; k < i; ++k) g += v_(i, k) * v_(k, j);
        for (size_t k = 0; k < i; ++k) v_(k, j) -= g * v_(k, i);
      }
    }

    d_[i] = v_(i, i);
    v_(i, i) = T(1);
    for (size_t j = 0; j < i; ++j) v_(j, i) = v_(i, j) = T(0);
  }
  
  for (size_t i = 1; i < n_; ++i) work[i - 1] = work[i];

  // Symmetric tridiagonal QL algorithm
  size_t sweeps = 40;
  for (size_t l = 0, m = 0; l < n_; ++l) {
    for (size_t it = 0; it <= sweeps; ++it) {
      // Find smallest subdiagonal element
      for (m = l; m < n_ - 1; ++m) {
        auto s = std::abs(d_[m]) + std::abs(d_[m + 1]);
        if (std::abs(work[m]) <= std::numeric_limits<T>::epsilon() * s)
          break;
      }

      // Check for convergence
      if (m == l) 
        break;
      if (it == sweeps) {
        throw std::logic_error{"Eigendecomposition failed (No convergence)"};
      }

      // Compute implicit shift
      auto d = (d_[l + 1] - d_[l]) / (T(2) * work[l]);
      auto g = (d_[m] - d_[l]) + work[l] / (d + sign(d) * std::hypot(d, T(1)));
      auto c = T(1), s = T(1), p = T(0);

      // Implicit QL transformation
      for (size_t i = m; i--> l; ) {
        auto b = c * work[i];
        auto f = s * work[i];
        auto r = work[i + 1] = std::hypot(f, g);

        if (r == T(0)) {
          d_[i + 1] -= p, work[m] = T(0);
          break;
        }

        s = f / r;
        c = g / r;
        g = d_[i + 1] - p;
        r = (d_[i] - g) * s + T(2) * c * b;
        d_[i + 1] = g + (p = s * r);
        g = c * r - b;

        // Accumulate transformation
        for (size_t k = 0; k < n_; ++k) {
          f = v_(k, i + 1);
          v_(k, i + 1) = s * v_(k, i) + c * f, v_(k, i) = c * v_(k, i) - s * f;
        }
      }

      d_[l] -= p;
      work[l] = g, work[m] = T(0);
    }
  }
}

namespace lin {

template <typename Lin>
constexpr auto eig_sym_run(const lin_base<Lin>& lin) {
  assert(lin.square() && "Eigendecomposition requires a square matrix");
  return eigen<Lin, true>{lin};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_EIGEN_H
