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

#ifndef IG_MATH_QR_H
#define IG_MATH_QR_H

#include "imagine/math/linalg/matrix.h"

namespace ig {

/* 
 Compute a QR factorization of a general M-by-N matrix A

 The factorization has the form
   A = Q R

 - Q is an orthogonal M-by-M matrix
 - R is an upper triangular M-by-N matrix

 Q is stored as a packed set of Householder transformations
 in the strict lower triangular part of the computed matrix
 R is stored in the diagonal and upper triangular part of the computed matrix
*/

template <typename Alg>
class qr {
public:
  using T = alg_t<Alg>;
  using matrix_type = matrix<T>;
  using vector_type = colvec<T>;

  static_assert(std::is_arithmetic<T>::value, "QR decomposition requires an arithmetic matrix");

  explicit qr(const matrix_type& alg);

  auto solve(const vector_type& b) -> vector_type;

  auto& mat() const { return qr_; }
  auto& tau() const { return tau_; }

private:
  const size_t m_, n_;

  matrix_type qr_;
  vector_type tau_;
};

template <typename Alg>
qr<Alg>::qr(const matrix_type& alg)
  : m_{alg.rows()}, n_{alg.cols()}, qr_{alg}, tau_{n_} {

  for (size_t i = 0; i < n_; ++i) {
    auto norm = T(0);
    for (size_t j = i; j < m_; ++j) norm = std::hypot(norm, qr_(j, i));

    if (norm != T(0)) {
      // Householder i-th reflector
      tau_[i] = -norm * sign(qr_(i, i));
      for (size_t j = i; j < m_; ++j) qr_(j, i) /= -tau_[i];

      // Transform remaining columns
      qr_(i, i) += T(1);
      for (size_t j = i + 1; j < n_; ++j) {
        auto s = T(0);
        for (size_t k = i; k < m_; ++k) s += qr_(k, i) * qr_(k, j);

        s = -s / qr_(i, i);
        for (size_t k = i; k < m_; ++k) qr_(k, j) += s * qr_(k, i);
      }
    }
  }
}

template <typename Alg>
auto qr<Alg>::solve(const vector_type& b) -> vector_type {
  // Compute y = Q^Tb
  auto y = b;
  for (size_t i = 0; i < n_; ++i) {
    auto s = T(0);
    for (size_t j = i; j < m_; ++j) s += qr_(j, i) * y[j];

    s = -s / qr_(i, i);
    for (size_t j = i; j < m_; ++j) y[j] += s * qr_(j, i);
  }

  // Backward Rx = y
  vector_type x{n_};
  for (size_t i = n_; i--> 0;) {
    x[i] = y[i] / tau_[i];
    for (size_t j = 0; j < i; ++j) y[j] -= x[i] * qr_(j, i);
  }
  return x;
}

namespace linalg {

template <typename Alg>
constexpr auto qr_run(const alg<Alg>& alg) {
  assert(alg.rows() >= alg.cols() && "QR decomposition requires a square or rectangular matrix"
                                     " where m >= n");
  return qr<Alg>{alg};
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_QR_H
