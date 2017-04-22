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

#ifndef IG_MATH_UNARY_H
#define IG_MATH_UNARY_H

#include "imagine/math/theory/detail/matrix/base.h"
#include <functional>

namespace ig {

template <typename Mat, typename Op>
struct mat_traits< unary_expr<Mat, Op> > {
  using type = mat_t<Mat>;
  static constexpr auto n_rows = Mat::n_rows, n_cols = Mat::n_cols;
};

template <typename Fn>
struct unary_fn {
  template <typename T> constexpr auto operator()(const T& val) const { return fn_(val); }
  Fn fn_;
};

template <typename Mat, typename Op>
class unary_expr : public matrix_base< unary_expr<Mat, Op> > {
public:
  explicit unary_expr(const Mat& mat, const Op& op)
    : mat_{mat}
    , op_{op} {}

  auto rows() const { return mat_.rows(); }
  auto cols() const { return mat_.cols(); }

  auto operator()(size_t row, size_t col) const { return op_(mat_(row, col)); }
  auto operator[](size_t n) const               { return op_(mat_[n]); }

private:
  const Mat mat_;
  const Op op_;
};

template <typename Mat>
constexpr auto operator-(const matrix_base<Mat>& mat) {
  return unary_expr< Mat, std::negate<> >{mat.derived(), std::negate<>{}};
}

// Unary lambda-based function applier
template <typename Mat, typename Fn>
constexpr auto apply_fn(const matrix_base<Mat>& mat, Fn&& fn) 
{ return unary_expr< Mat, unary_fn<Fn> >{mat.derived(), unary_fn<Fn>{fn}}; }

template <typename Mat>
constexpr auto operator+(const matrix_base<Mat>& mat, mat_t<Mat> scalar) 
{ return apply_fn(mat, [scalar](auto& val) { return val + scalar; }); }

template <typename Mat>
constexpr auto operator-(const matrix_base<Mat>& mat, mat_t<Mat> scalar) 
{ return apply_fn(mat, [scalar](auto& val) { return val - scalar; }); }

template <typename Mat>
constexpr auto operator-(mat_t<Mat> scalar, const matrix_base<Mat>& mat) 
{ return apply_fn(mat, [scalar](auto& val) { return scalar - val; }); }

template <typename Mat>
constexpr auto operator/(const matrix_base<Mat>& mat, mat_t<Mat> scalar) 
{ return apply_fn(mat, [scalar](auto& val) { return val / scalar; }); }

template <typename Mat>
constexpr auto operator/(mat_t<Mat> scalar, const matrix_base<Mat>& mat) 
{ return apply_fn(mat, [scalar](auto& val) { return scalar / val; }); }

template <typename Mat>
constexpr auto operator*(const matrix_base<Mat>& mat, mat_t<Mat> scalar) 
{ return apply_fn(mat, [scalar](auto& val) { return val * scalar; }); }

template <typename Mat>
constexpr auto operator*(mat_t<Mat> scalar, const matrix_base<Mat>& mat) 
{ return apply_fn(mat, [scalar](auto& val) { return scalar * val; }); }

} // namespace ig

#endif // IG_MATH_UNARY_H
