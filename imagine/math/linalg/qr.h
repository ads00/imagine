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

#ifndef IG_MATH_QR_H
#define IG_MATH_QR_H

#include "imagine/math/linalg/matrix.h"

namespace ig {

template <typename Lin>
class qr {
public:
  using type = lin_t<Lin>;
  using matrix_type = matrix<type>;
  using vector_type = colvec<type>;

  static_assert(std::is_arithmetic<type>::value, "QR decomposition requires an arithmetic matrix");

  explicit qr(const matrix_type& lin);

  auto solve(const vector_type& b) -> vector_type;

  auto& mat() const { return qr_; }
  auto& tau() const { return tau_; }

private:
  const size_t m_, n_;

  matrix_type qr_;
  vector_type tau_;
};

template <typename Lin>
qr<Lin>::qr(const matrix_type& lin)
  : m_{lin.rows()}
  , n_{lin.cols()}
  , qr_{lin}
  , tau_{n_} {

  for (size_t i = 0; i < n_; ++i) {
    type norm = 0;
    for (size_t j = i; j < m_; ++j) norm = std::hypot(norm, qr_(j, i));

    if (norm != 0) {
      // Householder i-th reflector
      tau_[i] = -norm * sign(qr_(i, i));
      for (size_t j = i; j < m_; ++j) qr_(j, i) /= -tau_[i];

      // Transform remaining columns
      qr_(i, i) += 1;
      for (size_t j = i + 1; j < n_; ++j) {
        type s = 0;
        for (size_t k = i; k < m_; ++k) s += qr_(k, i) * qr_(k, j);

        s = -s / qr_(i, i);
        for (size_t k = i; k < m_; ++k) qr_(k, j) += s * qr_(k, i);
      }
    }
  }
}

template <typename Lin>
auto qr<Lin>::solve(const vector_type& b) -> vector_type {
  // Compute y = Q^Tb
  auto y = b;
  for (size_t i = 0; i < n_; ++i) {
    type s = 0;
    for (size_t j = i; j < m_; ++j) s += qr_(j, i) * y[j];

    s = -s / qr_(i, i);
    for (size_t j = i; j < m_; ++j) y[j] += s * qr_(j, i);
  }

  // Backward Rx = y
  vector_type x{n_};
  for (size_t i = n_; i--> 0; ) {
    x[i] = y[i] / tau_[i];
    for (size_t j = 0; j < i; ++j) y[j] -= x[i] * qr_(j, i);
  }
  return x;
}

namespace lin {

template <typename Lin>
constexpr auto qr_run(const lin_base<Lin>& lin) {
  assert(lin.rows() >= lin.cols() && "QR decomposition requires a square or rectangular matrix"
                                     " where m >= n");
  return qr<Lin>{lin};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_QR_H
