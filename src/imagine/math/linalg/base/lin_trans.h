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

#ifndef IG_MATH_TRANS_H
#define IG_MATH_TRANS_H

#include "imagine/math/linalg/base/lin.h"

namespace ig {

template <typename Xpr>
struct lin_traits< lin_trans<Xpr> > {
  using T = lin_t<Xpr>;
  static constexpr auto M = Xpr::N, N = Xpr::M;
};

template <typename Xpr>
class lin_trans : public lin_base< lin_trans<Xpr> > {
public:
  explicit lin_trans(Xpr& xpr)
    : xpr_{xpr} {}

  auto rows() const { return xpr_.cols(); }
  auto cols() const { return xpr_.rows(); }

  auto operator()(size_t row, size_t col) const { return xpr_(col, row); }
  auto& operator()(size_t row, size_t col)      { return xpr_(col, row); }

  auto operator[](size_t) const = delete;
  auto& operator[](size_t)      = delete;

private:
  Xpr& xpr_;
};

} // namespace ig

#endif // IG_MATH_TRANS_H
