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

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/operation.h"
#include "imagine/math/linalg/lu.h"

namespace ig  {
namespace lin {

template <typename Lin>
constexpr auto norm(const lin_base<Lin>& lin) {
  return std::sqrt(dot(lin, lin));
}

template <typename Lin>
constexpr auto normalise(const lin_base<Lin>& lin) {
  return lin / norm(lin);
}

template <typename Lin>
constexpr auto trace(const lin_base<Lin>& lin) {
  return lin.diag().sum();
}

namespace detail {

template <typename Lin, size_t size = Lin::N>
struct determinant {
  static constexpr auto run(const lin_base<Lin>& lin) {
    return lu_run(lin).det();
  }
};

template <typename Lin>
struct determinant<Lin, 4> {
  static auto run(const lin_base<Lin>& lin) {
    auto det_helper = [&lin](size_t a, size_t b, size_t c, size_t d) {
      return (lin(a, 0) * lin(b, 1) - lin(b, 0) * lin(a, 1)) *
             (lin(c, 2) * lin(d, 3) - lin(d, 2) * lin(c, 3));
    };

    return det_helper(0, 1, 2, 3) -
           det_helper(0, 2, 1, 3) +
           det_helper(0, 3, 1, 2) +
           det_helper(1, 2, 0, 3) -
           det_helper(1, 3, 0, 2) +
           det_helper(2, 3, 0, 1);
  }
};

template <typename Lin>
struct determinant<Lin, 3> {
  static auto run(const lin_base<Lin>& lin) {
    auto det_helper = [&lin](size_t a, size_t b, size_t c) {
      return lin(0, a) * (lin(1, b) * lin(2, c) - lin(1, c) * lin(2, b));
    };

    return det_helper(0, 1, 2) -
           det_helper(1, 0, 2) +
           det_helper(2, 0, 1);
  }
};

template <typename Lin>
struct determinant<Lin, 2> {
  static auto run(const lin_base<Lin>& lin) {
    return lin(0, 0) * lin(1, 1) - lin(1, 0) * lin(0, 1);
  }
};

template <typename Lin, size_t size = Lin::M>
struct inverse {
  static constexpr auto run(const lin_base<Lin>& lin) {
    return lu_run(lin).inv();
  }
};

template <typename Lin>
struct inverse<Lin, 4> {
  static auto run(const lin_base<Lin>& lin) {
    auto cofactor = [&lin](size_t a, size_t b) {
      auto det3_helper = [&lin](size_t a1, size_t a2, size_t a3,
                                size_t b1, size_t b2, size_t b3) {
        return lin(a1, b1) * (lin(a2, b2) * lin(a3, b3) - lin(a2, b3) * lin(a3, b2));
      };

      auto a1 = (a + 1) % 4, a2 = (a + 2) % 4, a3 = (a + 3) % 4;
      auto b1 = (b + 1) % 4, b2 = (b + 2) % 4, b3 = (b + 3) % 4;

      return det3_helper(a1, a2, a3, b1, b2, b3) +
             det3_helper(a2, a3, a1, b1, b2, b3) +
             det3_helper(a3, a1, a2, b1, b2, b3);
    };

    typename Lin::plain_type 
      inv{};
    auto invdet = 1 / determinant<Lin>::run(lin);

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

template <typename Lin>
struct inverse<Lin, 3> {
  static auto run(const lin_base<Lin>& lin) {
    auto cofactor = [&lin](size_t a, size_t b) {
      auto a1 = (a + 1) % 3, a2 = (a + 2) % 3;
      auto b1 = (b + 1) % 3, b2 = (b + 2) % 3;

      return lin(a1, b1) * lin(a2, b2) - lin(a1, b2) * lin(a2, b1);
    };

    typename Lin::plain_type
      inv{};
    auto invdet = 1 / determinant<Lin>::run(lin);

    inv(0, 0) = cofactor(0, 0) * invdet; inv(1, 0) = cofactor(0, 1) * invdet; inv(2, 0) = cofactor(0, 2) * invdet;
    inv(0, 1) = cofactor(1, 0) * invdet; inv(1, 1) = cofactor(1, 1) * invdet; inv(2, 1) = cofactor(1, 2) * invdet;
    inv(0, 2) = cofactor(2, 0) * invdet; inv(1, 2) = cofactor(2, 1) * invdet; inv(2, 2) = cofactor(2, 2) * invdet;
    return inv;
  }
};

template <typename Lin>
struct inverse<Lin, 2> {
  static auto run(const lin_base<Lin>& lin) {
    typename Lin::plain_type
      inv{};
    auto invdet = 1 / determinant<Lin>::run(lin);

    inv(0, 0) =  lin(1, 1) * invdet; inv(1, 0) = -lin(1, 0) * invdet;
    inv(0, 1) = -lin(0, 1) * invdet; inv(1, 1) =  lin(0, 0) * invdet;
    return inv;
  }
};

} // namespace detail

template <typename Lin>
constexpr auto det(const lin_base<Lin>& lin) {
  assert(lin.square() && "Determinant exists only with square matrices");
  return detail::determinant<Lin>::run(lin);
}

template <typename Lin>
constexpr auto inv(const lin_base<Lin>& lin) {
  assert(lin.square() && "Inverse exists only with square matrices");
  return detail::inverse<Lin>::run(lin);
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_ANALYSIS_H
