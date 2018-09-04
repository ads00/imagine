/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_DET_H
#define IG_MATH_DET_H

#include "imagine/math/lin/decomposition/lu.h"

namespace ig  {
namespace lin {
namespace detail {

template <typename Mat, size_t n = matrix_traits<Mat>::n_cols>
struct determinant {
  static constexpr auto run(const matrix_base<Mat>& mat) {
    return
    lu_run(mat).det(); }
};

template <typename Mat>
struct determinant<Mat, 2> {
  static auto run(const matrix_base<Mat>& mat) {
    return
      mat(0, 0) * mat(1, 1) -
      mat(1, 0) * mat(0, 1);
  }
};

template <typename Mat>
struct determinant<Mat, 3> {
  static auto run(const matrix_base<Mat>& mat) {
    auto det_helper = [&mat](size_t a, size_t b, size_t c) {
      return mat(0, a) * (mat(1, b) * mat(2, c) - mat(1, c) * mat(2, b));
    };

    return
      det_helper(0, 1, 2) -
      det_helper(1, 0, 2) +
      det_helper(2, 0, 1);
  }
};

template <typename Mat>
struct determinant<Mat, 4> {
  static auto run(const matrix_base<Mat>& mat) {
    auto det_helper = [&mat](size_t a, size_t b, size_t c, size_t d) {
      return (mat(a, 0) * mat(b, 1) - mat(b, 0) * mat(a, 1)) *
             (mat(c, 2) * mat(d, 3) - mat(d, 2) * mat(c, 3));
    };

    return
      det_helper(0, 1, 2, 3) -
      det_helper(0, 2, 1, 3) +
      det_helper(0, 3, 1, 2) +
      det_helper(1, 2, 0, 3) -
      det_helper(1, 3, 0, 2) +
      det_helper(2, 3, 0, 1);
  }
};

} // namespace detail

template <typename Mat>
constexpr auto det(const matrix_base<Mat>& mat) {
  assert(mat.square() && "Determinant exists only with square matrices");
  return
    detail::determinant<Mat>::run
    (mat);
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_DET_H
