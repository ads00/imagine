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

#ifndef IG_MATH_LU_H
#define IG_MATH_LU_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/solver/direct.h"

namespace ig {

template <typename Lin>
class lu {
public:
  using type = lin_t<Lin>;
  using matrix_type = matrix<type>;
  using vector_type = colvec<type>;

  static_assert(std::is_arithmetic<type>::value, "LU decomposition requires an arithmetic matrix");

  explicit lu(const matrix_type& lin);

  auto det() const -> type;
  auto inv() const -> matrix_type;
  auto solve(const vector_type& b) const -> vector_type;

  auto& mat() const   { return lu_; }
  auto& perms() const { return p_; }

private:
  const size_t n_;
  size_t permutations_;

  matrix_type lu_, p_;
};

template <typename Lin>
lu<Lin>::lu(const matrix_type& lin)
  : n_{lin.diagsize()}
  , permutations_{0}
  , lu_{lin}
  , p_{matrix_type::eye(n_)} {

  for (size_t i = 0, row = i; i < n_; ++i) {
    // Find largest pivot element
    type pivot = 0;
    for (size_t j = i; j < n_; ++j) {
      auto curr_elemt = std::abs(lu_(j, i));
      if (curr_elemt > pivot)
        pivot = curr_elemt, row = j;
    }

    if (pivot == 0) {
      throw std::logic_error{"LU decomposition failed (Singular matrix)"};
    }

    // Partial row pivoting
    if (row != i) {
      permutations_++;
      for (size_t j = 0; j < n_; ++j)
        std::swap(lu_(row, j), lu_(i, j)), 
        std::swap(p_ (row, j), p_ (i, j));
    }

    // Factorize
    auto f = lu_(i, i);
    for (size_t j = i + 1; j < n_; ++j) {
      auto g = lu_(j, i) /= f;
      for (size_t k = i + 1; k < n_; ++k) lu_(j, k) -= g * lu_(i, k);
    }
  }
}

template <typename Lin>
auto lu<Lin>::det() const -> type {
  auto detsign = (permutations_ % 2) 
    ? -1 
    :  1;
  return detsign * lu_.diag().prod();
}

template <typename Lin>
auto lu<Lin>::inv() const -> matrix_type {
  // Solve for each column on eye matrix
  auto inv = p_;
  for (size_t i = 0; i < n_; ++i)
    lin::forward_solve (lu_, inv.col(i), true),
    lin::backward_solve(lu_, inv.col(i));
  return inv;
}

template <typename Lin>
auto lu<Lin>::solve(const vector_type& b) const -> vector_type {
  vector_type x{p_ * b};
  lin::forward_solve (lu_, x, true);
  lin::backward_solve(lu_, x);
  return x;
}

namespace lin {

template <typename Lin>
constexpr auto lu_run(const lin_base<Lin>& lin) {
  assert(lin.square() && "LU decomposition requires a square matrix");
  return lu<Lin>{lin};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_LU_H
