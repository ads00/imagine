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

#ifndef IG_MATH_ANALYSIS_H
#define IG_MATH_ANALYSIS_H

#include "imagine/math/theory/matrix.h"
#include "imagine/math/linalg/operation.h"
#include "imagine/math/linalg/lu.h"

namespace ig  {
namespace lin {

template <typename Mat>
constexpr auto norm(const matrix_base<Mat>& mat) {
  return std::sqrt(dot(mat, mat));
}

template <typename Mat>
constexpr auto normalise(const matrix_base<Mat>& mat) {
  return mat / norm(mat);
}

template <typename Mat>
constexpr auto trace(const matrix_base<Mat>& mat) {
  return mat.diag().sum();
}

namespace detail {

template <typename Mat, size_t size = Mat::n_cols>
struct determinant {
  static constexpr auto run(const matrix_base<Mat>& mat) {
    return lu_run(mat).det();
  }
};

template <typename Mat>
struct determinant<Mat, 4> {
  static auto run(const matrix_base<Mat>& mat) {
    auto det_helper = [&mat](size_t a, size_t b, size_t c, size_t d) {
      return (mat(a, 0) * mat(b, 1) - mat(b, 0) * mat(a, 1)) *
             (mat(c, 2) * mat(d, 3) - mat(d, 2) * mat(c, 3));
    };

    return det_helper(0, 1, 2, 3) -
           det_helper(0, 2, 1, 3) +
           det_helper(0, 3, 1, 2) +
           det_helper(1, 2, 0, 3) -
           det_helper(1, 3, 0, 2) +
           det_helper(2, 3, 0, 1);
  }
};

template <typename Mat>
struct determinant<Mat, 3> {
  static auto run(const matrix_base<Mat>& mat) {
    auto det_helper = [&mat](size_t a, size_t b, size_t c) {
      return mat(0, a) * (mat(1, b) * mat(2, c) - mat(1, c) * mat(2, b));
    };

    return det_helper(0, 1, 2) -
           det_helper(1, 0, 2) +
           det_helper(2, 0, 1);
  }
};

template <typename Mat>
struct determinant<Mat, 2> {
  static auto run(const matrix_base<Mat>& mat) {
    return mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1);
  }
};

template <typename Mat, size_t size = Mat::n_rows>
struct inverse {
  static constexpr auto run(const matrix_base<Mat>& mat) {
    return lu_run(mat).inv();
  }
};

template <typename Mat>
struct inverse<Mat, 4> {
  static auto run(const matrix_base<Mat>& mat) {
    auto cofactor = [&mat](size_t a, size_t b) {
      auto det3_helper = [&mat](size_t a1, size_t a2, size_t a3,
                                size_t b1, size_t b2, size_t b3) {
        return mat(a1, b1) * (mat(a2, b2) * mat(a3, b3) - mat(a2, b3) * mat(a3, b2));
      };

      auto a1 = (a + 1) % 4, a2 = (a + 2) % 4, a3 = (a + 3) % 4;
      auto b1 = (b + 1) % 4, b2 = (b + 2) % 4, b3 = (b + 3) % 4;

      return det3_helper(a1, a2, a3, b1, b2, b3) +
             det3_helper(a2, a3, a1, b1, b2, b3) +
             det3_helper(a3, a1, a2, b1, b2, b3);
    };

    typename Mat::plain_type 
      inv{};
    auto invdet = 1 / determinant<Mat>::run(mat);

    inv(0, 0) =  cofactor(0, 0) * invdet; inv(1, 0) = -cofactor(0, 1) * invdet;
    inv(2, 0) =  cofactor(0, 2) * invdet; inv(3, 0) = -cofactor(0, 3) * invdet;
    inv(0, 1) = -cofactor(1, 0) * invdet; inv(1, 1) =  cofactor(1, 1) * invdet;
    inv(2, 1) = -cofactor(1, 2) * invdet; inv(3, 1) =  cofactor(1, 3) * invdet;
    inv(0, 2) =  cofactor(2, 0) * invdet; inv(1, 2) = -cofactor(2, 1) * invdet;
    inv(2, 2) =  cofactor(2, 2) * invdet; inv(3, 2) = -cofactor(2, 3) * invdet;
    inv(0, 3) = -cofactor(3, 0) * invdet; inv(1, 3) =  cofactor(3, 1) * invdet;
    inv(2, 3) = -cofactor(3, 2) * invdet; inv(3, 3) =  cofactor(3, 3) * invdet;
    return inv;
  }
};

template <typename Mat>
struct inverse<Mat, 3> {
  static auto run(const matrix_base<Mat>& mat) {
    auto cofactor = [&mat](size_t a, size_t b) {
      auto a1 = (a + 1) % 3, a2 = (a + 2) % 3;
      auto b1 = (b + 1) % 3, b2 = (b + 2) % 3;

      return mat(a1, b1) * mat(a2, b2) - mat(a1, b2) * mat(a2, b1);
    };

    typename Mat::plain_type
      inv{};
    auto invdet = 1 / determinant<Mat>::run(mat);

    inv(0, 0) = cofactor(0, 0) * invdet; inv(1, 0) = cofactor(0, 1) * invdet; inv(2, 0) = cofactor(0, 2) * invdet;
    inv(0, 1) = cofactor(1, 0) * invdet; inv(1, 1) = cofactor(1, 1) * invdet; inv(2, 1) = cofactor(1, 2) * invdet;
    inv(0, 2) = cofactor(2, 0) * invdet; inv(1, 2) = cofactor(2, 1) * invdet; inv(2, 2) = cofactor(2, 2) * invdet;
    return inv;
  }
};

template <typename Mat>
struct inverse<Mat, 2> {
  static auto run(const matrix_base<Mat>& mat) {
    typename Mat::plain_type
      inv{};
    auto invdet = 1 / determinant<Mat>::run(mat);

    inv(0, 0) =  mat(1, 1) * invdet; inv(1, 0) = -mat(1, 0) * invdet;
    inv(0, 1) = -mat(0, 1) * invdet; inv(1, 1) =  mat(0, 0) * invdet;
    return inv;
  }
};

} // namespace detail

template <typename Mat>
constexpr auto det(const matrix_base<Mat>& mat) {
  assert(mat.square() && "Determinant exists only with square matrices");
  return detail::determinant<Mat>::run(mat);
}

template <typename Mat>
constexpr auto inv(const matrix_base<Mat>& mat) {
  assert(mat.square() && "Inverse exists only with square matrices");
  return detail::inverse<Mat>::run(mat);
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_ANALYSIS_H
