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

#ifndef IG_MATH_SVD_H
#define IG_MATH_SVD_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/equation.h"

namespace ig {

/* 
 Compute a SVD factorization of a general M-by-N matrix A

 The factorization has the form
   A = U D V^T

 - U is a column-orthogonal M-by-N matrix
 - D is a diagonal N-by-N matrix
 - V is an orthogonal N-by-N matrix

 U is stored in a computed matrix and contains the left singular vectors
 D is stored in a vector, and contains the singular values
 V is stored in a computed matrix and contains the right singular vectors
*/

template <typename Alg>
class svd {
public:
  using T = alg_t<Alg>;
  using matrix_t = matrix<T>;
  using vector_t = vector<T>;

  static_assert(std::is_arithmetic<T>::value, 
                "Singular value decomposition requires an arithmetic matrix");

  svd(const matrix_t& alg);

  size_t rank() const;
  auto pinv() const -> matrix_t;
  auto solve(const vector_t& b) -> vector_t;

  constexpr auto& matrixU() const         { return U_; }
  constexpr auto& matrixV() const         { return V_; }
  constexpr auto& singular_values() const { return S_; }

private:
  const size_t M_;
  const size_t N_;

  T threshold_;

  matrix_t U_;
  matrix_t V_;
  vector_t S_;
};

template <typename Alg>
svd<Alg>::svd(const matrix_t& alg)
  : M_{alg.rows()}, N_{alg.cols()}, threshold_{T(0)}, U_{alg}, V_{N_, N_}, S_{N_} {

  auto work = vector_t{N_};
  auto g = T(0);
  // Householder's reduction to bidiagonal form
  for (size_t i = 0; i < N_; ++i) {
    auto l = i + 1;
    work[i] = g;

    auto s1 = T(0);
    for (size_t j = i; j < M_; ++j) s1 += U_(j, i) * U_(j, i);

    if (s1 > std::numeric_limits<T>::epsilon()/*eps<T>*/) {
      auto f = U_(i, i);
      g = -sign(f) * std::sqrt(s1);

      auto h = f*g - s1;
      U_(i, i) = f - g;

      for (size_t j = l; j < N_; ++j) {
        auto s = T(0);
        for (size_t k = i; k < M_; ++k) s += U_(k, i) * U_(k, j);

        f = s / h;
        for (size_t k = i; k < M_; ++k) U_(k, j) += f * U_(k, i);
      }
    } else g = T(0);

    S_[i] = g;
    if (i >= M_) continue;

    auto s2 = T(0);
    for (size_t j = l; j < N_; ++j) s2 += U_(i, j) * U_(i, j);

    if (s2 > std::numeric_limits<T>::epsilon()/*eps<T>*/) {
      auto f = U_(i, l);
      g = -sign(f) * std::sqrt(s2);

      auto h = f*g - s2;
      U_(i, l) = f - g;
      for (size_t j = l; j < N_; ++j) work[j] = U_(i, j) / h;

      for (size_t j = l; j < M_; ++j) {
        auto s = T(0);
        for (size_t k = l; k < N_; ++k) s += U_(j, k) * U_(i, k);
        for (size_t k = l; k < N_; ++k) U_(j, k) += s * work[k];
      }
    } else g = T(0);
  }

  // Accumulation of right hand transformations
  for (size_t i = N_; i--> 0;) {
    if (g != T(0)) {
      auto h = g * U_(i, i + 1);
      for (size_t j = i + 1; j < N_; ++j) V_(j, i) = U_(i, j) / h;

      for (size_t j = i + 1; j < N_; ++j) {
        auto s = T(0);
        for (size_t k = i + 1; k < N_; ++k) s += U_(i, k) * V_(k, j);
        for (size_t k = i + 1; k < N_; ++k) V_(k, j) += s * V_(k, i);
      }
    }

    for (size_t j = i + 1; j < N_; ++j) V_(i, j) = V_(j, i) = T(0);

    V_(i, i) = T(1);
    g = work[i];
  }

  // Accumulation of left hand transformations
  for (size_t i = std::min(M_, N_); i--> 0;) {
    auto l = i + 1;
    for (size_t j = l; j < N_; ++j) U_(i, j) = T(0);

    if (S_[i] != T(0)) {
      auto h = U_(i, i) * S_[i];
      for (size_t j = l; j < N_; ++j) {
        auto s = T(0);
        for (size_t k = l; k < M_; ++k) s += U_(k, i) * U_(k, j);

        auto f = s / h;
        for (size_t k = i; k < M_; ++k) U_(k, j) += f * U_(k, i);
      }

      for (size_t j = i; j < M_; ++j) U_(j, i) /= S_[i];
    } else {
      for (size_t j = i; j < M_; ++j) U_(j, i) = T(0);
    }

    U_(i, i)++;
  }

  auto sweeps = 75;
  threshold_ = std::numeric_limits<T>::epsilon()/*eps<T>*/ * std::max(M_, N_) * std::abs(S_[0]);

  // Diagonalization of the bidiagonal form
  for (size_t i = N_; i--> 0; ) {
    auto l = size_t{0}, nm = size_t{0};
    for (size_t it = 0; it <= sweeps; ++it) {

      // Test for splitting
      auto split = true;
      for (l = i + 1, nm = l - 1; l--> 0; ) {
        if (l == 0 || std::abs(work[l]) <= threshold_) {
          split = false;
          break;
        }
        if (std::abs(S_[nm]) <= threshold_)
          break;
      }

      if (split) {
        auto c = T(0), s = T(1);
        for (size_t j = l; j < i + 1; ++j) {
          auto f = s * work[j];
          if (std::abs(f) <= threshold_) 
            break;

          auto g = S_[j];
          auto h = std::hypot(f, g);

          S_[j] = h;
          c = g / h;
          s = -f / h;

          for (size_t k = 0; k < M_; ++k) {
            auto y = U_(k, nm), z = U_(k, j);
            U_(k, nm) = y*c + z*s, U_(k, j)  = z*c - y*s;
          }
        }
      }

      auto z = S_[i];
      // Convergence
      if (l == i) { 
        if (z < T(0)) {
          S_[i] = -z;
          for (size_t j = 0; j < N_; ++j) V_(j, i) = -V_(j, i);
        } break;
      }

      // No convergence after x sweeps
      if (it == sweeps) {
        throw std::logic_error("Singular value decomposition failed (No convergence)");
      }

      // Shift from bottom 2x2 minor
      auto y = S_[i - 1];
      auto h = work[i];
      auto g = work[i - 1];

      auto x = S_[l];
      auto f = ((y - z)*(y + z) + (g - h)*(g + h)) / (T(2) * h * y);
      g = std::hypot(f, T(1));
      f = ((x - z)*(x + z) + h*((y / (f + sign(f)*g)) - h)) / x;

      auto c = T(1), s = T(1);

      // Next QR transformation
      for (size_t j = l; j < i; ++j) {
        auto j1 = j + 1;
        auto g1 = c * work[j1], h1 = s * work[j1], y1 = S_[j1];
        auto z1 = std::hypot(f, h1);

        work[j] = z1;
        c = f / z1;
        s = h1 / z1;
        f = x*c + g1*s;

        auto g2 = g1*c - x*s, h2 = y1 * s, y2 = y1 * c;
        auto z2 = std::hypot(f, h2);

        for (size_t k = 0; k < N_; ++k) {
          auto x = V_(k, j), z = V_(k, j1);
          V_(k, j)  = x*c + z*s, V_(k, j1) = z*c - x*s;
        }

        S_[j] = z2;
        if (z2 != T(0)) {
          c = f / z2, s = h2 / z2;
        }

        f = c*g2 + s*y2;
        x = c*y2 - s*g2;

        for (size_t k = 0; k < M_; ++k) {
          auto y = U_(k, j), z = U_(k, j1);
          U_(k, j)  = y*c + z*s, U_(k, j1) = z*c - y*s;
        }
      }

      work[l] = T(0), work[i] = f;
      S_[i] = x;
    }
  }
}

template <typename Alg>
size_t svd<Alg>::rank() const {
  // Lookup for singular values > threshold 
  auto r = size_t{0};
  for (size_t i = 0; i < N_; ++i) {
    if (S_[i] > threshold_) r++;
  }
  return r;
}

template <typename Alg>
auto svd<Alg>::pinv() const -> matrix_t {
  // Compute w = VS+
  auto w = matrix_t{N_, N_};
  for (size_t i = 0; i < N_; ++i)
    for (size_t j = 0; j < N_; ++j)
      if (std::abs(S_[j]) > threshold_) w(i, j) = V_(i, j) * (T(1) / S_[j]);
  // Compute inv = wU^T
  return w * U_.t();
}

template <typename Alg>
auto svd<Alg>::solve(const vector_t& b) -> vector_t {
  // Compute w = U^Tb
  auto w = vector_t{U_.t() * b};

  // Apply singularity
  for (size_t i = 0; i < N_; ++i) {
    auto sv = S_[i];
    auto alpha = std::abs(sv) > threshold_ ? T(1) / sv : sv;
    w[i] *= alpha;
  }
  // Compute x = Vw
  return V_ * w;
}

namespace linalg {

template <typename Alg>
constexpr svd<Alg> svd_run(const alg<Alg>& alg) {
  return svd<Alg>(alg);
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_SVD_H
