/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_QR_H
#define IG_MATH_QR_H

#include "imagine/math/theory/matrix.h"

namespace ig {

template <typename Mat>
class qr {
public:
  using value_type = matrix_t<Mat>;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;

  static_assert(std::is_arithmetic<value_type>::value, "QR decomposition requires an arithmetic matrix");

  explicit qr(const matrix_type& mat);

  bool fullrank() const;
  auto solve(const vector_type& b) const -> vector_type;

  auto& mat() const { return qr_; }
  auto& tau() const { return tau_; }

private:
  const size_t m_, n_;

  value_type threshold_;

  matrix_type qr_;
  vector_type tau_;
};

template <typename Mat>
qr<Mat>::qr(const matrix_type& mat)
  : m_{mat.rows()}
  , n_{mat.cols()}
  , threshold_{std::numeric_limits<value_type>::epsilon() * m_}
  , qr_{mat}
  , tau_{n_} {

  for (size_t i = 0; i < n_; ++i) {
    value_type s = 0;
    for (size_t j = i; j < m_; ++j) s = std::hypot(s, qr_(j, i));

    if (s != 0) {
      // Householder i-th reflector
      tau_[i] = -s * sign(qr_(i, i));
      for (size_t j = i; j < m_; ++j) qr_(j, i) /= -tau_[i];

      // Transform remaining columns
      qr_(i, i) += 1;
      for (size_t j = i + 1; j < n_; ++j) {
        s = 0;
        for (size_t k = i; k < m_; ++k) s += qr_(k, i) * qr_(k, j);
        s = -s / qr_(i, i);
        for (size_t k = i; k < m_; ++k) qr_(k, j) += s * qr_(k, i);
      }
    }
  }
}

template <typename Mat>
bool qr<Mat>::fullrank() const {
  return std::find_if(tau_.begin(), tau_.end(),
  [this](auto v) { return std::abs(v) < threshold_; }) == tau_.end();
}

template <typename Mat>
auto qr<Mat>::solve(const vector_type& b) const -> vector_type {
  // Compute y = Q^Tb
  auto y = b;
  for (size_t i = 0; i < n_; ++i) {
    value_type s = 0;
    for (size_t j = i; j < m_; ++j) s += qr_(j, i) * y[j];
    s = -s / qr_(i, i);
    for (size_t j = i; j < m_; ++j) y[j] += s * qr_(j, i);
  }

  // Backward Rx = y
  vector_type x{n_};
  for (size_t i = n_; i-- > 0; ) {
    x[i] = y[i] / tau_[i];
    for (size_t j = 0; j < i; ++j) y[j] -= x[i] * qr_(j, i);
  }
  return x;
}

namespace lin {

template <typename Mat>
constexpr auto qr_run(const matrix_base<Mat>& mat) {
  assert(mat.rows() >= mat.cols() && "QR decomposition requires a square or rectangular matrix"
                                     " where m >= n");
  return qr<Mat>{mat};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_QR_H
