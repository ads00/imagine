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

#ifndef IG_MATH_SVD_H
#define IG_MATH_SVD_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/solver/direct.h"

namespace ig {

template <typename Lin>
class svd {
public:
  using type = lin_t<Lin>;
  using matrix_type = matrix<type>;
  using vector_type = colvec<type>;

  static_assert(std::is_arithmetic<type>::value, "Singular value decomposition requires an arithmetic matrix");

  explicit svd(const matrix_type& lin);

  size_t rank() const;
  auto pinv() const -> matrix_type;
  auto solve(const vector_type& b) -> vector_type;

  auto& matU() const           { return u_; }
  auto& matV() const           { return v_; }
  auto& singularvalues() const { return s_; }

private:
  const size_t m_, n_;

  type threshold_;

  matrix_type u_, v_;
  vector_type s_;
};

template <typename Lin>
svd<Lin>::svd(const matrix_type& lin)
  : m_{lin.rows()}
  , n_{lin.cols()}
  , threshold_{0}
  , u_{lin}
  , v_{n_, n_}
  , s_{n_} {

  vector_type work{n_};
  type g = 0;
  // Householder's reduction to bidiagonal form
  for (size_t i = 0; i < n_; ++i) {
    auto l = i + 1;
    work[i] = g;

    type s1 = 0;
    for (size_t j = i; j < m_; ++j) s1 += u_(j, i) * u_(j, i);

    if (s1 > std::numeric_limits<type>::epsilon()) {
      auto f = u_(i, i);
      g = -sign(f) * std::sqrt(s1);

      auto h = f * g - s1;
      u_(i, i) = f - g;

      for (size_t j = l; j < n_; ++j) {
        auto s = 0;
        for (size_t k = i; k < m_; ++k) s += u_(k, i) * u_(k, j);

        f = s / h;
        for (size_t k = i; k < m_; ++k) u_(k, j) += f * u_(k, i);
      }
    } else g = 0;

    s_[i] = g;
    if (i >= m_) 
      continue;

    type s2 = 0;
    for (size_t j = l; j < n_; ++j) s2 += u_(i, j) * u_(i, j);

    if (s2 > std::numeric_limits<type>::epsilon()) {
      auto f = u_(i, l);
      g = -sign(f) * std::sqrt(s2);

      auto h = f * g - s2;
      u_(i, l) = f - g;
      for (size_t j = l; j < n_; ++j) work[j] = u_(i, j) / h;
      for (size_t j = l; j < m_; ++j) {
        type s = 0;
        for (size_t k = l; k < n_; ++k) s += u_(j, k) * u_(i, k);
        for (size_t k = l; k < n_; ++k) u_(j, k) += s * work[k];
      }
    } else g = 0;
  }

  // Accumulation of right hand transformations
  for (size_t i = n_; i--> 0; ) {
    if (g != 0) {
      auto h = g * u_(i, i + 1);
      for (size_t j = i + 1; j < n_; ++j) v_(j, i) = u_(i, j) / h;
      for (size_t j = i + 1; j < n_; ++j) {
        type s = 0;
        for (size_t k = i + 1; k < n_; ++k) s += u_(i, k) * v_(k, j);
        for (size_t k = i + 1; k < n_; ++k) v_(k, j) += s * v_(k, i);
      }
    }

    for (size_t j = i + 1; j < n_; ++j) v_(i, j) = v_(j, i) = 0;

    v_(i, i) = 1;
    g = work[i];
  }

  // Accumulation of left hand transformations
  for (size_t i = std::min(m_, n_); i--> 0; ) {
    auto l = i + 1;
    for (size_t j = l; j < n_; ++j) u_(i, j) = 0;

    if (s_[i] != 0) {
      auto h = u_(i, i) * s_[i];
      for (size_t j = l; j < n_; ++j) {
        type s = 0;
        for (size_t k = l; k < m_; ++k) s += u_(k, i) * u_(k, j);

        auto f = s / h;
        for (size_t k = i; k < m_; ++k) u_(k, j) += f * u_(k, i);
      }

      for (size_t j = i; j < m_; ++j) u_(j, i) /= s_[i];
    } else {
      for (size_t j = i; j < m_; ++j) u_(j, i) = 0;
    }

    u_(i, i)++;
  }

  size_t sweeps = 75;
  threshold_ = std::numeric_limits<T>::epsilon() * std::max(m_, n_) * std::abs(s_[0]);

  // Diagonalization of the bidiagonal form
  for (size_t i = n_; i--> 0; ) {
    size_t l = 0, nm = 0;
    for (size_t it = 0; it <= sweeps; ++it) {

      // Test for splitting
      bool split = true;
      for (l = i + 1, nm = l - 1; l--> 0; ) {
        if (l == 0 || std::abs(work[l]) <= threshold_) {
          split = false;
          break;
        }
        if (std::abs(s_[nm]) <= threshold_)
          break;
      }

      if (split) {
        type c = 0, s = 1;
        for (size_t j = l; j < i + 1; ++j) {
          auto f = s * work[j];
          if (std::abs(f) <= threshold_) 
            break;

          auto g = s_[j];
          auto h = std::hypot(f, g);

          s_[j] = h;
          c = g / h;
          s = -f / h;

          for (size_t k = 0; k < m_; ++k) {
            auto y = u_(k, nm), z = u_(k, j);
            u_(k, nm) = y * c + z * s, u_(k, j) = z * c - y * s;
          }
        }
      }

      auto z = s_[i];
      // Convergence
      if (l == i) { 
        if (z < 0) {
          s_[i] = -z;
          for (size_t j = 0; j < n_; ++j) v_(j, i) = -v_(j, i);
        } break;
      }

      // No convergence after x sweeps
      if (it == sweeps) {
        throw std::logic_error{"Singular value decomposition failed (No convergence)"};
      }

      // Shift from bottom 2x2 minor
      auto y = s_[i - 1];
      auto h = work[i];
      auto g = work[i - 1];

      auto x = s_[l];
      auto f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2 * h * y);
      g = std::hypot(f, 1);
      f = ((x - z) * (x + z) + h * ((y / (f + sign(f) * g)) - h)) / x;

      type c = 1, s = 1;

      // Next QR transformation
      for (size_t j = l; j < i; ++j) {
        auto j1 = j + 1;
        auto g1 = c * work[j1], h1 = s * work[j1], y1 = s_[j1];
        auto z1 = std::hypot(f, h1);

        work[j] = z1;
        c = f / z1;
        s = h1 / z1;
        f = x * c + g1 * s;

        auto g2 = g1 * c - x * s, h2 = y1 * s, y2 = y1 * c;
        auto z2 = std::hypot(f, h2);

        for (size_t k = 0; k < n_; ++k) {
          auto x = v_(k, j), z = v_(k, j1);
          v_(k, j) = x * c + z * s, v_(k, j1) = z * c - x * s;
        }

        s_[j] = z2;
        if (z2 != 0) c = f / z2, s = h2 / z2;

        f = c * g2 + s * y2;
        x = c * y2 - s * g2;

        for (size_t k = 0; k < m_; ++k) {
          auto y = u_(k, j), z = u_(k, j1);
          u_(k, j) = y * c + z * s, u_(k, j1) = z * c - y * s;
        }
      }

      work[l] = 0, work[i] = f;
      s_[i] = x;
    }
  }
}

template <typename Lin>
size_t svd<Lin>::rank() const {
  // Lookup for singular values > threshold 
  size_t r = 0;
  for (size_t i = 0; i < n_; ++i)
    if (s_[i] > threshold_) r++;
  return r;
}

template <typename Lin>
auto svd<Lin>::pinv() const -> matrix_type {
  // Compute w = VS+
  matrix_type w{n_, n_};
  for (size_t i = 0; i < n_; ++i)
    for (size_t j = 0; j < n_; ++j)
      if (std::abs(s_[j]) > threshold_) w(i, j) = v_(i, j) * (1 / s_[j]);
  // Compute inv = wU^T
  return w * u_.t();
}

template <typename Lin>
auto svd<Lin>::solve(const vector_type& b) -> vector_type {
  // Compute w = U^Tb
  vector_type w{u_.t() * b};

  // Apply singularity
  for (size_t i = 0; i < n_; ++i) {
    auto sv = s_[i];
    auto alpha = (std::abs(sv) > threshold_) 
      ? 1 / sv 
      : sv;
    w[i] *= alpha;
  }
  // Compute x = Vw
  return v_ * w;
}

namespace lin {

template <typename Lin>
constexpr auto svd_run(const lin_base<Lin>& lin) {
  return svd<Lin>{lin};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_SVD_H