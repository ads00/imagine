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

#include "imagine/math/theory/matrix.h"

namespace ig {

template 
< typename Mat, 
  bool Symmetric = false > class eigen;

// Symmetric eigendecomposition
template <typename Mat>
class eigen<Mat, true> {
public:
  using value_type = matrix_t<Mat>;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;

  static_assert(std::is_arithmetic<value_type>::value, "Eigendecomposition requires an arithmetic matrix");

  explicit eigen(const matrix_type& mat);

  auto& evt() const { return v_; }
  auto& evl() const { return d_; }

private:
  const size_t n_;

  matrix_type v_;
  vector_type d_;
};

template <typename Mat>
eigen<Mat, true>::eigen(const matrix_type& mat)
  : n_{mat.diagsize()}
  , v_{mat}
  , d_{n_} {

  vector_type e{n_};
  value_type  s = 0;
  // Symmetric Householder reduction to tridiagonal form
  for (size_t i = n_; i-- > 1; ) {
    
    if (i - 1 > 0) {
      // Generate Householder vector
      for (size_t j = 0; j < i; ++j) s += v_(i, j) * v_(i, j);

      auto f = v_(i, i - 1);
      auto g = sign(f) * std::sqrt(s);

      e[i] = g;
      s -= f * g;

      v_(i, i - 1) = f - g;
      f = 0;

      // Apply similarity transformation to remaining columns
      for (size_t j = 0; j < i; ++j) {
        v_(j, i) = v_(i, j) / s;
        g = 0;
        for (size_t k = 0; k < j + 1; ++k) g += v_(j, k) * v_(i, k);
        for (size_t k = j + 1; k < i; ++k) g += v_(k, j) * v_(i, k);

        e[j] = g / s;
        f += e[j] * v_(i, j);
      }

      auto ss = f / (s + s);
      for (size_t j = 0; j < i; ++j) {
        f = v_(i, j);
        g = e[j] -= ss * f;
        for (size_t k = 0; k < j + 1; ++k) v_(j, k) -= f * e[k] + g * v_(i, k);
      }
    } else e[i] = v_(i, i - 1);

    d_[i] = s;
  }

  d_[0] = e[0] = 0;

  // Accumulate transformations
  for (size_t i = 0; i < n_; ++i) {
    if (d_[i] != 0) {
      for (size_t j = 0; j < i; ++j) {
        s = 0;
        for (size_t k = 0; k < i; ++k) s += v_(i, k) * v_(k, j);
        for (size_t k = 0; k < i; ++k) v_(k, j) -= s * v_(k, i);
      }
    }

    d_[i] = v_(i, i);
    v_(i, i) = 1;
    for (size_t j = 0; j < i; ++j) v_(j, i) = v_(i, j) = 0;
  }
  
  for (size_t i = 1; i < n_; ++i) e[i - 1] = e[i];

  // Symmetric tridiagonal QL algorithm
  size_t sweeps = 40;
  for (size_t l = 0, m = 0; l < n_; ++l) {
    for (size_t iter = 0; iter <= sweeps; ++iter) {
      // Find smallest subdiagonal element
      for (m = l; m < n_ - 1; ++m) {
        auto s = std::abs(d_[m]) + std::abs(d_[m + 1]);
        if (std::abs(e[m]) <= std::numeric_limits<value_type>::epsilon() * s)
          break;
      }

      // Check for convergence
      if (m == l) 
        break;
      if (iter == sweeps) {
        throw std::logic_error{"Eigendecomposition failed (No convergence)"};
      }

      // Compute implicit shift
      auto d = (d_[l + 1] - d_[l]) / (2 * e[l]);
      auto g = (d_[m] - d_[l]) + e[l] / (d + sign(d) * std::hypot(d, value_type(1)));
      value_type
        c = 1, 
        s = 1, 
        p = 0;

      // Implicit QL transformation
      for (size_t i = m; i-- > l; ) {
        auto b = c * e[i];
        auto f = s * e[i];
        auto r = e[i + 1] = std::hypot(f, g);

        if (r == 0) {
          d_[i + 1] -= p, e[m] = 0;
          break; }

        s = f / r;
        c = g / r;
        g = d_[i + 1] - p;
        r = (d_[i] - g) * s + 2 * c * b;
        d_[i + 1] = g + (p = s * r);
        g = c * r - b;

        // Accumulate transformation
        for (size_t k = 0; k < n_; ++k) {
          f = v_(k, i + 1);
          v_(k, i + 1) = s * v_(k, i) + c * f, v_(k, i) = c * v_(k, i) - s * f; }
      }

      e[m] = 0;
      e[l] = g;
      d_[l] -= p;
    }
  }
}

namespace lin {

template <typename Mat>
constexpr auto eigs_run(const matrix_base<Mat>& mat) {
  assert(mat.square() && "Eigendecomposition requires a square matrix");
  return eigen<Mat, true>{mat};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_EIGEN_H
