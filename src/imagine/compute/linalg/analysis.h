/*
 Copyright (c) 2015, 2016
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

#ifndef COMPUTE_ANALYSIS_H
#define COMPUTE_ANALYSIS_H

#include "imagine/compute/linalg/matrix.h"
#include "imagine/compute/linalg/operation.h"
#include "imagine/compute/linalg/lu.h"

namespace ig     {
namespace linalg {

template <typename Alg>
constexpr auto norm(const alg<Alg>& alg)
{
  return std::sqrt(dot(alg, alg));
}

template <typename Alg>
constexpr auto normalise(const alg<Alg>& alg)
{
  return alg / norm(alg);
}

template <typename Alg>
constexpr auto trace(const alg<Alg>& alg)
{
  return alg.diag().sum();
}

namespace detail
{

template <typename Alg, std::size_t size = Alg::N>
struct determinant
{
  static constexpr auto run(const alg<Alg>& alg)
  {
    return lu_run(alg).det();
  }
};

template <typename Alg>
struct determinant<Alg, 4>
{
  static auto run(const alg<Alg>& alg)
  {
    const auto det_helper = [&alg](std::size_t a, std::size_t b, std::size_t c, std::size_t d)
    {
      return (alg(a, 0)*alg(b, 1) - alg(b, 0)*alg(a, 1)) *
             (alg(c, 2)*alg(d, 3) - alg(d, 2)*alg(c, 3));
    };

    return det_helper(0, 1, 2, 3) -
           det_helper(0, 2, 1, 3) +
           det_helper(0, 3, 1, 2) +
           det_helper(1, 2, 0, 3) -
           det_helper(1, 3, 0, 2) +
           det_helper(2, 3, 0, 1);
  }
};

template <typename Alg>
struct determinant<Alg, 3>
{
  static auto run(const alg<Alg>& alg)
  {
    const auto det_helper = [&alg](std::size_t a, std::size_t b, std::size_t c)
    {
      return alg(0, a) * (alg(1, b)*alg(2, c) - alg(1, c)*alg(2, b));
    };

    return det_helper(0, 1, 2) -
           det_helper(1, 0, 2) +
           det_helper(2, 0, 1);
  }
};

template <typename Alg>
struct determinant<Alg, 2>
{
  static auto run(const alg<Alg>& alg)
  {
    return alg(0, 0)*alg(1, 1) - alg(1, 0)*alg(0, 1);
  }
};

template <typename Alg, std::size_t size = Alg::M>
struct inverse
{
  static constexpr typename Alg::plain_t run(const alg<Alg>& alg)
  {
    return lu_run(alg).inv();
  }
};

template <typename Alg>
struct inverse<Alg, 4>
{
  static auto run(const alg<Alg>& alg)
  {
    const auto cofactor = [&alg](std::size_t a, std::size_t b)
    {
      const auto det3_helper = [&alg](std::size_t a1, std::size_t a2, std::size_t a3,
                                      std::size_t b1, std::size_t b2, std::size_t b3)
      {
        return alg(a1, b1) * (alg(a2, b2)*alg(a3, b3) - alg(a2, b3)*alg(a3, b2));
      };

      const std::size_t a1 = (a + 1) % 4, a2 = (a + 2) % 4, a3 = (a + 3) % 4;
      const std::size_t b1 = (b + 1) % 4, b2 = (b + 2) % 4, b3 = (b + 3) % 4;

      return det3_helper(a1, a2, a3, b1, b2, b3) +
             det3_helper(a2, a3, a1, b1, b2, b3) +
             det3_helper(a3, a1, a2, b1, b2, b3);
    };

    using T = Alg::T;

    Alg::plain_t inv;
    const T invdet = T(1) / det(alg);

    inv(0, 0) = cofactor(0, 0) * invdet; inv(1, 0) = -cofactor(0, 1) * invdet;
    inv(2, 0) = cofactor(0, 2) * invdet; inv(3, 0) = -cofactor(0, 3) * invdet;

    inv(0, 1) = -cofactor(1, 0) * invdet; inv(1, 1) = cofactor(1, 1) * invdet;
    inv(2, 1) = -cofactor(1, 2) * invdet; inv(3, 1) = cofactor(1, 3) * invdet;

    inv(0, 2) = cofactor(2, 0) * invdet; inv(1, 2) = -cofactor(2, 1) * invdet;
    inv(2, 2) = cofactor(2, 2) * invdet; inv(3, 2) = -cofactor(2, 3) * invdet;

    inv(0, 3) = -cofactor(3, 0) * invdet; inv(1, 3) = cofactor(3, 1) * invdet;
    inv(2, 3) = -cofactor(3, 2) * invdet; inv(3, 3) = cofactor(3, 3) * invdet;

    return inv;
  }
};

template <typename Alg>
struct inverse<Alg, 3>
{
  static auto run(const alg<Alg>& alg)
  {
    const auto cofactor = [&alg](std::size_t a, std::size_t b)
    {
      const std::size_t a1 = (a + 1) % 3, a2 = (a + 2) % 3;
      const std::size_t b1 = (b + 1) % 3, b2 = (b + 2) % 3;

      return alg(a1, b1)*alg(a2, b2) - alg(a1, b2)*alg(a2, b1);
    };

    using T = Alg::T;

    Alg::plain_t inv;
    const T invdet = T(1) / det(alg);

    inv(0, 0) = cofactor(0, 0) * invdet; inv(1, 0) = cofactor(0, 1) * invdet; inv(2, 0) = cofactor(0, 2) * invdet;
    inv(0, 1) = cofactor(1, 0) * invdet; inv(1, 1) = cofactor(1, 1) * invdet; inv(2, 1) = cofactor(1, 2) * invdet;
    inv(0, 2) = cofactor(2, 0) * invdet; inv(1, 2) = cofactor(2, 1) * invdet; inv(2, 2) = cofactor(2, 2) * invdet;

    return inv;
  }
};

template <typename Alg>
struct inverse<Alg, 2>
{
  static auto run(const alg<Alg>& alg)
  {
    using T = Alg::T;

    Alg::plain_t inv;
    const T invdet = T(1) / det(alg);

    inv(0, 0) = alg(1, 1) * invdet; inv(1, 0) = -alg(1, 0) * invdet;
    inv(0, 1) = -alg(0, 1) * invdet; inv(1, 1) = alg(0, 0) * invdet;
    return inv;
  }
};

} // namespace detail

template <typename Alg>
constexpr auto det(const alg<Alg>& alg)
{
  assert(alg.square() && "Determinant exists only with square matrices");
  return detail::determinant<Alg>::run(alg);
}

template <typename Alg>
constexpr auto inv(const alg<Alg>& alg)
{
  assert(alg.square() && "Inverse exists only with square matrices");
  return detail::inverse<Alg>::run(alg);
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_ANALYSIS_H