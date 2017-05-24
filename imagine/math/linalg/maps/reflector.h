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

#ifndef IG_MATH_REFLECTOR_H
#define IG_MATH_REFLECTOR_H

#include "imagine/math/theory/matrix.h"
#include "imagine/math/linalg/operation.h"

namespace ig  {

template <typename Vec>
class reflector {
public:
  using type = mat_t<Vec>;
  static_assert(std::is_arithmetic<type>::value, "Elementary reflector computation requires an arithmetic vector");

  explicit reflector(Vec& vec);

  auto& tau() const  { return tau_; }
  auto& beta() const { return beta_; }

private:
  Vec& vec_;
  type tau_, beta_;
};

template <typename Vec>
reflector<Vec>::reflector(Vec& vec)
  : vec_{vec} {

  auto c = vec_[0];
  auto s = lin::dot(vec_, vec_);

  beta_ = -sign(c) * std::sqrt(s); tau_ = (beta_ - c) / beta_;
  for (size_t i = 1; i < vec.size(); ++i) vec_[i] /= c - beta_;
}

namespace lin {

template <typename Vec>
constexpr auto householder(Vec& vec) {
  assert(vec.vector() && "Householder reflection requires a vector");
  return reflector<Vec>{vec};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_REFLECTOR_H
