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

#ifndef IG_MATH_INV_H
#define IG_MATH_INV_H

#include "imagine/math/linalg/det.h"
#include "imagine/math/linalg/factor/lu.h"

namespace ig  {
namespace lin {
namespace detail {

template
< typename Mat,
  size_t n = matrix_traits<Mat>::n_rows >
struct inverse {
  static constexpr auto run(const matrix_base<Mat>& mat) { return lu_run(mat).inv(); }
};

template <typename Mat>
struct inverse<Mat, 2> {
  static auto run(const matrix_base<Mat>& mat) {
    return
      1 / det(mat) *
      concrete_matrix<Mat>{ mat(1, 1), -mat(0, 1),
                           -mat(1, 0),  mat(0, 0)};
  }
};

template <typename Mat>
struct inverse<Mat, 3> {
  static auto com(const matrix_base<Mat>& mat, size_t a, size_t b) {
    auto a1 = (a + 1) % 3,
         a2 = (a + 2) % 3;
    auto b1 = (b + 1) % 3,
         b2 = (b + 2) % 3;

    return
      mat(a1, b1) * mat(a2, b2) -
      mat(a1, b2) * mat(a2, b1);
  }

  static auto run(const matrix_base<Mat>& mat) {
    return
      1 / det(mat) *
      concrete_matrix<Mat>{com(mat, 0, 0), com(mat, 1, 0), com(mat, 2, 0),
                           com(mat, 0, 1), com(mat, 1, 1), com(mat, 2, 1),
                           com(mat, 0, 2), com(mat, 1, 2), com(mat, 2, 2)};
  }
};

template <typename Mat>
struct inverse<Mat, 4> {
  static auto com(const matrix_base<Mat>& mat, size_t a, size_t b) {
    auto a1 = (a + 1) % 4,
         a2 = (a + 2) % 4,
         a3 = (a + 3) % 4;
    auto b1 = (b + 1) % 4,
         b2 = (b + 2) % 4,
         b3 = (b + 3) % 4;

    return
      mat(a1, b1) *
        (mat(a2, b2) * mat(a3, b3) - mat(a2, b3) * mat(a3, b2)) +
      mat(a2, b1) *
        (mat(a3, b2) * mat(a1, b3) - mat(a3, b3) * mat(a1, b2)) +
      mat(a3, b1) *
        (mat(a1, b2) * mat(a2, b3) - mat(a1, b3) * mat(a2, b2));
  }

  static auto run(const matrix_base<Mat>& mat) {
    return
      1 / det(mat) *
      concrete_matrix<Mat>{ com(mat, 0, 0), -com(mat, 1, 0),  com(mat, 2, 0), -com(mat, 3, 0),
                           -com(mat, 0, 1),  com(mat, 1, 1), -com(mat, 2, 1),  com(mat, 3, 1),
                            com(mat, 0, 2), -com(mat, 1, 2),  com(mat, 2, 2), -com(mat, 3, 2),
                           -com(mat, 0, 3),  com(mat, 1, 3), -com(mat, 2, 3),  com(mat, 3, 3)};
  }
};

} // namespace detail

template <typename Mat>
constexpr auto inv(const matrix_base<Mat>& mat) {
  assert(mat.square() && "Inverse exists only with square matrices");
  return
    detail::inverse<Mat>::run
    (mat);
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_INV_H
