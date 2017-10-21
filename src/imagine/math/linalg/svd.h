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

#include "imagine/math/theory/matrix.h"
#include "imagine/math/linalg/solver/direct.h"

namespace ig {

template <typename Mat>
class svd {
public:
  using value_type = matrix_t<Mat>;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;

  static_assert(std::is_arithmetic<value_type>::value, "Singular value decomposition requires an arithmetic matrix");

  explicit svd(const matrix_type& mat);

  size_t rank() const;
  auto nrm2() const -> value_type;
  auto pinv() const -> matrix_type;
  auto solve(const vector_type& b) -> vector_type;

  auto& matU() const { return u_; }
  auto& matV() const { return v_; }
  auto& svl() const  { return s_; }

private:
  const size_t m_, n_;

  value_type threshold_;

  matrix_type u_, v_;
  vector_type s_;
};

template <typename Mat>
svd<Mat>::svd(const matrix_type& mat)
  : m_{mat.rows()}
  , n_{mat.cols()}
  , threshold_{0}
  , u_{mat}
  , v_{n_, n_}
  , s_{n_} {

  vector_type e{n_};
  value_type
    g = 0, 
    s = 0;
  // Householder's reduction to bidiagonal form
  for (size_t i = 0; i < n_; ++i) {
    auto l = i + 1; e[i] = g;
    
    s = 0;
    for (size_t j = i; j < m_; ++j) s += u_(j, i) * u_(j, i);

    auto f = u_(i, i);
    g = -sign(f) * sqrt(s);
    auto h = f * g - s;
    u_(i, i) = f - g;
    for (size_t j = l; j < n_; ++j) {
      s = 0;
      for (size_t k = i; k < m_; ++k) s += u_(k, i) * u_(k, j);
      f = s / h;
      for (size_t k = i; k < m_; ++k) u_(k, j) += f * u_(k, i);
    } s_[i] = g;

    if (i < n_ - 1) {
      s = 0;
      for (size_t j = l; j < n_; ++j) s += u_(i, j) * u_(i, j);

      auto f = u_(i, l);
      g = -sign(f) * sqrt(s);
      auto h = f * g - s;
      u_(i, l) = f - g;
      for (size_t j = l; j < n_; ++j) e[j] = u_(i, j) / h;
      for (size_t j = l; j < m_; ++j) {
        s = 0;
        for (size_t k = l; k < n_; ++k) s += u_(j, k) * u_(i, k);
        for (size_t k = l; k < n_; ++k) u_(j, k) += s * e[k];
      }
    }
  }

  // Accumulation of right hand transformations
  for (size_t i = n_; i-- > 0; ) {
    for (size_t j = i + 1; j < n_; ++j) v_(j, i) = u_(i, j) / u_(i, i + 1) / g;
    for (size_t j = i + 1; j < n_; ++j) {
      s = 0;
      for (size_t k = i + 1; k < n_; ++k) s += u_(i, k) * v_(k, j);
      for (size_t k = i + 1; k < n_; ++k) v_(k, j) += s * v_(k, i);
    }

    for (size_t j = i + 1; j < n_; ++j) v_(i, j) = v_(j, i) = 0;
    v_(i, i) = 1; g = e[i];
  }

  // Accumulation of left hand transformations
  for (size_t i = n_; i-- > 0; ) {
    auto l = i + 1;
    for (size_t j = l; j < n_; ++j) u_(i, j) = 0;

    if (s_[i] != 0) {
      auto h = 1 / s_[i];
      for (size_t j = l; j < n_; ++j) {
        s = 0;
        for (size_t k = l; k < m_; ++k) s += u_(k, i) * u_(k, j);
        auto f = s / u_(i, i) * h;
        for (size_t k = i; k < m_; ++k) u_(k, j) += f * u_(k, i);
      }
      for (size_t j = i; j < m_; ++j) u_(j, i) *= h;
    } else {
      for (size_t j = i; j < m_; ++j) u_(j, i)  = 0;
    }
    u_(i, i)++;
  }

  size_t sweeps = 75;
  threshold_ = std::numeric_limits<value_type>::epsilon() * m_ * std::abs(s_[0]);

  // Diagonalization of the bidiagonal form
  for (size_t i = n_; i-- > 0; ) {
    size_t l = 0, nm = 0;
    value_type
      s = 1, 
      f = 0, 
      c = 0;

    for (size_t iter = 0; iter < sweeps; ++iter) {
      // Test for splitting
      bool split = true;
      for (l = i + 1; l--> 0; ) {
        nm = l - 1;
        if (std::abs(e[l]) < threshold_) {
          split = false;
          break;
        } else if (std::abs(s_[nm]) < threshold_) 
          break;
      }

      if (split) {
        for (size_t j = l; j < i + 1; ++j) {
          f = s * e[j];
          if (std::abs(f) > threshold_) {
            auto g = s_[j];
            auto h = std::hypot(f, g);

            s_[j] = h; 
            h = 1 / h;
            c = g * h; s = -f * h;
            for (size_t k = 0; k < m_; ++k) {
              auto y = u_(k, nm),
                   z = u_(k,  j);
              u_(k, nm) = y * c + z * s; u_(k, j) = z * c - y * s;
            }
          }
        }
      }

      auto z = s_[i];
      if (l == i) {
        // Convergence
        if (z < 0) {
          s_[i] = -z;
          for (size_t j = 0; j < n_; ++j) v_(j, i) = -v_(j, i);
        } break;
      }

      // No convergence after x sweeps
      if (iter > sweeps) {
        throw std::logic_error{"Singular value decomposition failed (No convergence)"};
      }

      // Shift from bottom 2x2 minor
      auto x = s_[l];
      auto y = s_[i - 1];

      auto h = e[i];
      auto g = e[i - 1];
      
      f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2 * h * y);
      g = std::hypot(f, value_type(1));
      f = ((x - z) * (x + z) + h * ((y / (f + sign(f) * g)) - h)) / x;
    
      c = s = 1;
      // Next QR transformation
      for (size_t j = l; j < i; ++j) {
        auto t = j + 1;
        h = s * e[t];
        g = c * e[t];
        z = std::hypot(f, h);
        e[j] = z;
        c = f / z;
        s = h / z;

        f = x * c + g * s; g = g * c - x * s;
        h = s_[t] * s;
        y = s_[t] * c;
        for (size_t k = 0; k < n_; ++k) {
          auto x = v_(k, j),
               z = v_(k, t);
          v_(k, j) = x * c + z * s; v_(k, t) = z * c - x * s; }

        z = std::hypot(f, h);
        s_[j] = z;
        if (z != 0) {
          z = 1 / z;
          c = f * z; s = h * z; }

        f = c * g + s * y; x = c * y - s * g;
        for (size_t k = 0; k < m_; ++k) {
          auto y = u_(k, j),
               z = u_(k, t);
          u_(k, j) = y * c + z * s; u_(k, t) = z * c - y * s; }
      }

      e[l] = 0;
      e[i] = f;
      s_[i] = x;
    }
  }
}

template <typename Mat>
size_t svd<Mat>::rank() const {
  // Lookup for singular values > threshold 
  return std::count_if(s_.begin(), s_.end(), [this](auto v) { return v > threshold_; });
}

template <typename Mat>
auto svd<Mat>::nrm2() const -> value_type {
  // Max singular value
  return *std::max_element(s_.begin(), s_.end());
}

template <typename Mat>
auto svd<Mat>::pinv() const -> matrix_type {
  // Compute w = VS+
  matrix_type w{n_, n_};
  for (size_t i = 0; i < n_; ++i)
    for (size_t j = 0; j < n_; ++j)
      if (std::abs(s_[j]) > threshold_) w(i, j) = v_(i, j) * (1 / s_[j]);
  // Compute inv = wU^T
  return w * u_.t();
}

template <typename Mat>
auto svd<Mat>::solve(const vector_type& b) -> vector_type {
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

template <typename Mat>
constexpr auto svd_run(const matrix_base<Mat>& mat) {
  assert(mat.rows() >= mat.cols() && "SV decomposition requires a square or rectangular matrix"
                                     " where m >= n");
  return svd<Mat>{mat};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_SVD_H
