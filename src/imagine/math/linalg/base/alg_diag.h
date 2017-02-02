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

#ifndef IG_MATH_DIAG_H
#define IG_MATH_DIAG_H

#include "imagine/math/linalg/base/alg.h"

namespace ig {

template <typename Xpr>
struct alg_traits< alg_diag<Xpr> > : alg_traits<Xpr> {
  using T = alg_t<Xpr>;
  static constexpr auto M = Xpr::M, N = 1;
};

template <typename Xpr>
class alg_diag : public alg< alg_diag<Xpr> > {
public:
  explicit alg_diag(Xpr& xpr)
    : xpr_{xpr} {}

  auto rows() const { return xpr_.diagsize(); }
  auto cols() const { return 1; }

  auto operator()(size_t row, size_t) const { return xpr_(row, row); }
  auto& operator()(size_t row, size_t)      { return xpr_(row, row); }

  auto operator[](size_t n) const { return xpr_(n, n); }
  auto& operator[](size_t n)      { return xpr_(n, n); }

  auto operator=(const alg_diag& o) {
    eval(*this, o); return *this;
  }

  template <typename Alg>
  auto operator=(const alg<Alg>& o) {
    eval(*this, o); return *this;
  }

private:
  Xpr& xpr_;
};

} // namespace ig

#endif // IG_MATH_DIAG_H
