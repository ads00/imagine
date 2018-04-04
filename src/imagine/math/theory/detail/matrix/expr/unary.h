/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MATRIXUNARY_H
#define IG_MATH_MATRIXUNARY_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template
< typename Mat,
  typename FunctionObject >
struct matrix_traits
<
  matrix_unary<Mat, FunctionObject>
>
{
  using value_type = matrix_t<Mat>;
  static constexpr auto n_rows = matrix_traits<Mat>::n_rows, n_cols = matrix_traits<Mat>::n_cols;
};

template <typename Callable>
struct unary_operator {
  Callable fn_;
  template <typename T> constexpr auto operator()(const T& val) const { return fn_(val); } };

template
< typename m_,
  typename f_ >
class matrix_unary : public matrix_base< matrix_unary<m_, f_> > {
public:
  explicit matrix_unary(const m_& mat, const f_& op)
    : mat_{mat}
    , op_{op} {}

  auto rows() const { return mat_.rows(); }
  auto cols() const { return mat_.cols(); }

  auto operator()(size_t row, size_t col) const
  { return op_(mat_(row, col)); }
  auto operator[](size_t n) const
  { return op_(mat_[n]); }

private:
  m_ mat_;
  f_ op_;
};

template <typename Mat>
constexpr auto operator-(const matrix_base<Mat>& mat)
{ return matrix_unary< Mat, std::negate<> >{mat.derived(), std::negate<>{}}; }

// Unary lambda-based function operator
template <typename Mat, typename Callable>
constexpr auto u_expr(const matrix_base<Mat>& mat, Callable&& fn) {
  return matrix_unary
    < Mat,
      unary_operator<Callable>
    >{mat.derived(), unary_operator<Callable>{fn}};
}

} // namespace ig

#endif // IG_MATH_MATRIXUNARY_H
