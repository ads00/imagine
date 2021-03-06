/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_LU_H
#define IG_MATH_LU_H

#include "imagine/math/theory/matrix.h"
#include "imagine/math/lin/solver/direct.h"

namespace ig {

template <typename Mat>
class lu {
public:
  using value_type = matrix_t<Mat>;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;

  static_assert(std::is_arithmetic<value_type>::value, "LU decomposition requires an arithmetic matrix");

  explicit lu(const matrix_type& mat);

  auto det() const -> value_type;
  auto inv() const -> matrix_type;
  auto solve(const vector_type& b) const -> vector_type;

  auto& mat() const   { return lu_; }
  auto& perms() const { return p_; }

private:
  const size_t n_;
  size_t permutations_;

  matrix_type lu_, p_;
};

template <typename Mat>
lu<Mat>::lu(const matrix_type& mat)
  : n_{mat.diag_size()}
  , permutations_{0}
  , lu_{mat}
  , p_{matrix_type::eye(n_)} {

  for (size_t i = 0, r = i; i < n_; ++i) {
    // Find largest pivot element
    value_type p = 0;
    for (size_t j = i; j < n_; ++j) {
      auto c = std::abs(lu_(j, i));
      if (c > p)
        p = c,
        r = j;
    }

    if (p == 0) {
      throw std::logic_error{"LU decomposition failed (Singular matrix)"};
    }

    // Partial row pivoting
    if (r != i) {
      permutations_++;
      for (size_t j = 0; j < n_; ++j)
        std::swap(lu_(r, j), lu_(i, j)),
        std::swap(p_ (r, j), p_ (i, j));
    }

    // Factorize
    auto f = lu_(i, i);
    for (size_t j = i + 1; j < n_; ++j) {
      auto g = lu_(j, i) /= f;
      for (size_t k = i + 1; k < n_; ++k) lu_(j, k) -= g * lu_(i, k);
    }
  }
}

template <typename Mat>
auto lu<Mat>::det() const -> value_type {
  auto detsign = (permutations_ % 2)
    ? -1
    :  1;
  return detsign * lu_.diag().prod();
}

template <typename Mat>
auto lu<Mat>::inv() const -> matrix_type {
  // Solve for each column on eye matrix
  auto inv = p_;
  for (size_t i = 0; i < n_; ++i)
    lin::forward_solve(
      lu_,
      inv.col(i),
      true),
    lin::backward_solve(
      lu_,
      inv.col(i));
  return inv;
}

template <typename Mat>
auto lu<Mat>::solve(const vector_type& b) const -> vector_type {
  vector_type x{p_ % b};
  lin::forward_solve(
    lu_,
    x,
    true);
  lin::backward_solve(
    lu_,
    x);
  return x;
}

namespace lin {

template <typename Mat>
constexpr auto lu_run(const matrix_base<Mat>& mat) {
  assert(mat.square() && "LU decomposition requires a square matrix");
  return lu<Mat>{mat};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_LU_H
