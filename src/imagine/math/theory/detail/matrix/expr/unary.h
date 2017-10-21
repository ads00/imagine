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
  const m_ mat_;
  const f_ op_;
};

template <typename Mat>
constexpr auto operator-(const matrix_base<Mat>& mat)
{ return matrix_unary< Mat, std::negate<> >{mat.derived(), std::negate<>{}}; }

// Unary lambda-based function operator
template <typename Mat, typename Callable>
constexpr auto operator_expr(const matrix_base<Mat>& mat, Callable&& fn) {
  return matrix_unary
    < Mat, 
      unary_operator<Callable>
    >{mat.derived(), unary_operator<Callable>{fn}};
}

template <typename Mat>
constexpr auto operator+(const matrix_base<Mat>& lhs, matrix_t<Mat> rhs) 
{ return operator_expr(lhs, [rhs](auto& val) { return val + rhs; }); }

template <typename Mat>
constexpr auto operator+(matrix_t<Mat> lhs, const matrix_base<Mat>& rhs) 
{ return operator_expr(lhs, [lhs](auto& val) { return lhs + val; }); }

template <typename Mat>
constexpr auto operator-(const matrix_base<Mat>& lhs, matrix_t<Mat> rhs) 
{ return operator_expr(lhs, [rhs](auto& val) { return val - rhs; }); }

template <typename Mat>
constexpr auto operator-(matrix_t<Mat> lhs, const matrix_base<Mat>& rhs) 
{ return operator_expr(lhs, [lhs](auto& val) { return lhs - val; }); }

template <typename Mat>
constexpr auto operator*(const matrix_base<Mat>& lhs, matrix_t<Mat> rhs) 
{ return operator_expr(lhs, [rhs](auto& val) { return val * rhs; }); }

template <typename Mat>
constexpr auto operator*(matrix_t<Mat> lhs, const matrix_base<Mat>& rhs)
{ return operator_expr(rhs, [lhs](auto& val) { return lhs * val; }); }

template <typename Mat>
constexpr auto operator/(const matrix_base<Mat>& lhs, matrix_t<Mat> rhs)
{ return operator_expr(lhs, [rhs](auto& val) { return val / rhs; }); }

template <typename Mat>
constexpr auto operator/(matrix_t<Mat> lhs, const matrix_base<Mat>& rhs)
{ return operator_expr(rhs, [lhs](auto& val) { return lhs / val; }); }

} // namespace ig

#endif // IG_MATH_MATRIXUNARY_H
