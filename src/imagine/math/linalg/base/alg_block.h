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

#ifndef IG_MATH_BLOCK_H
#define IG_MATH_BLOCK_H

#include "imagine/math/linalg/base/alg.h"

namespace ig {

template <typename Xpr>
struct alg_traits< alg_block<Xpr> > : alg_traits<Xpr> {
  using T = alg_t<Xpr>;
  static constexpr auto M = Xpr::M, N = Xpr::N;
};

template <typename Xpr>
class alg_block : public alg< alg_block<Xpr> > {
public:
  explicit alg_block(Xpr& xpr, size_t row, size_t col, size_t nr, size_t nc)
    : xpr_{xpr}
    , colvec_{false}
    , rowvec_{false}
    , row_{row}
    , col_{col}
    , nr_{nr}
    , nc_{nc} {}

  explicit alg_block(Xpr& xpr, size_t start, size_t n)
    : xpr_{xpr}
    , colvec_{xpr_.cols() == 1}
    , rowvec_{xpr_.rows() == 1}
    , row_{colvec_ ? start : 0}
    , col_{rowvec_ ? start : 0}
    , nr_{colvec_ ? n : 1}
    , nc_{rowvec_ ? n : 1} {}

  auto rows() const { return nr_; }
  auto cols() const { return nc_; }

  auto operator()(size_t row, size_t col) const { return xpr_(row_ + row, col_ + col); }
  auto& operator()(size_t row, size_t col)      { return xpr_(row_ + row, col_ + col); }

  auto operator[](size_t n) const = delete;
  auto& operator[](size_t n)      = delete;

  auto operator=(const alg_block& o) {
    eval(*this, o); return *this;
  }

  template <typename Alg>
  auto operator=(const alg<Alg>& o) {
    eval(*this, o); return *this;
  }

private:
  Xpr& xpr_;
  bool colvec_, rowvec_;
  size_t row_, col_, nr_, nc_;
};

} // namespace ig

#endif // IG_MATH_BLOCK_H
