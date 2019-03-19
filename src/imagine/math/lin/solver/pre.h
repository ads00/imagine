/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_PRE_H
#define IG_MATH_PRE_H

#include "imagine/math/theory/matrix.h"

namespace ig {

template 
< typename Cond,
  typename Step >
class preconditioner {
public:
  using value_type = float;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;

  static_assert(std::is_arithmetic<value_type>::value, "Preconditioner requires an arithmetic matrix");

  explicit preconditioner(
    const matrix_type& mat, 
    size_t m, 
    size_t n, 
    Cond&& cond, 
    Step&& step)
    : full_{m, n}
    , cond_{cond}
    , step_{step}
  { cond_(full_, mat); }
  auto step(const vector_type& b) const { return step_(full_, b); }

protected:
  matrix_type full_;
  Cond cond_;
  Step step_;
};

namespace pre {

template <typename Mat>
auto diag(const matrix_base<Mat>& mat) {
  return preconditioner
  {
    mat,
    mat.diag_size(),
    1,
    [](auto& cond, auto& A) { for (size_t i = 0; i < A.diag_size(); ++i) cond[i] = A(i, i) != 0 ? 1 / A(i, i) : 1; },
    [](auto& cond, auto& b) { return cond * b; }
  };
}

} // namespace pre
} // namespace ig

#endif // IG_MATH_PRE_H
