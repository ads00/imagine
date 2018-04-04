/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_CHOLESKY_H
#define IG_MATH_CHOLESKY_H

#include "imagine/math/theory/matrix.h"
#include "imagine/math/linalg/solver/direct.h"

namespace ig {

template <typename Mat>
class cholesky {
public:
  using value_type = matrix_t<Mat>;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;

  static_assert(std::is_arithmetic<value_type>::value, "Cholesky decomposition requires an arithmetic matrix");

  explicit cholesky(const matrix_type& mat);

  auto det() const -> value_type;
  auto inv() const -> matrix_type;
  auto solve(const vector_type& b) const -> vector_type;

  auto& mat() const { return llt_; }

private:
  const size_t n_;
  matrix_type llt_;
};

template <typename Mat>
cholesky<Mat>::cholesky(const matrix_type& mat)
  : n_{mat.diagsize()}
  , llt_{mat} {

  for (size_t i = 0; i < n_; ++i) {
    for (size_t j = i; j < n_; ++j) {
      auto s = llt_(i, j);
      for (size_t k = i; k--> 0; ) s -= llt_(i, k) * llt_(j, k);

      if (i == j) {
        // Diagonal square root
        if (s <= std::numeric_limits<value_type>::epsilon()) {
          throw std::logic_error{"Cholesky decomposition failed (Not positive-definite)"};
        }
        llt_(i, i) = std::sqrt(s);
      } else {
        llt_(j, i) = s / llt_(i, i);
        llt_(i, j) = 0;
      }
    }
  }
}

template <typename Mat>
auto cholesky<Mat>::det() const -> value_type {
  return std::pow(llt_.diag().prod(), 2);
}

template <typename Mat>
auto cholesky<Mat>::inv() const -> matrix_type {
  // Forward L-1
  auto inv = matrix_type::eye(n_);
  for (size_t i = 0; i < b_; ++i)
    lin::forward_solve(
      llt_,
      inv.col(i));
  return inv.t() % inv;
}

template <typename Mat>
auto cholesky<Mat>::solve(const vector_type& b) const -> vector_type {
  vector_type x{b};
  lin::forward_solve (
    llt_,
    x);
  lin::backward_solve(
    llt_.t(),
    x);
  return x;
}

namespace lin {

template <typename Mat>
constexpr auto chol_run(const matrix_base<Mat>& mat) {
  assert(mat.square() && "Cholesky decomposition requires a square matrix");
  return cholesky<Mat>{mat};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_CHOLESKY_H
