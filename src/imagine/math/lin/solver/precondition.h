/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_PRECONDITION_H
#define IG_MATH_PRECONDITION_H

#include "imagine/math/theory/matrix.h"

namespace ig {

template <typename Arithmetic>
class jacobi_preconditioner {
public:
  using value_type = Arithmetic;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;

  static_assert(std::is_arithmetic<value_type>::value, "Jacobi preconditioner requires an arithmetic matrix");

  explicit jacobi_preconditioner(const matrix_type& mat)
    : invdiag_{mat.diagsize()} {

    for (size_t i = 0; i < mat.diagsize(); ++i)
      invdiag_[i] = mat(i, i) != 0
        ? 1 / mat(i, i)
        : 1;
  }

  auto solve(const vector_type& b) const -> vector_type;

private:
  vector_type invdiag_;
};

template <typename Arithmetic>
auto jacobi_preconditioner<Arithmetic>::solve(const vector_type& b) const -> vector_type
{ return invdiag_ * b; }

} // namespace ig

#endif // IG_MATH_PRECONDITION_H
