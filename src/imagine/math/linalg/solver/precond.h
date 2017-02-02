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

#ifndef IG_MATH_PRECOND_H
#define IG_MATH_PRECOND_H

#include "imagine/math/linalg/matrix.h"

namespace ig {

template <typename T>
class jacobi_precond {
public:
  using matrix_type = matrix<T>;
  using vector_type = colvec<T>;

  static_assert(std::is_arithmetic<T>::value,
                "Jacobi preconditioner requires an arithmetic matrix");

  explicit jacobi_precond(const matrix_type& mat)
    : invdiag_{mat.diagsize()} {

    for (size_t i = 0; i < mat.diagsize(); ++i)
      invdiag_[i] = mat(i, i) != 0 
        ? T(1) / mat(i, i) 
        : T(1);
  }

  vector_type solve(const vector_type& b) const {
    return invdiag_ % b;
  }

private:
  vector_type invdiag_;
};

} // namespace ig

#endif // IG_MATH_PRECOND_H
