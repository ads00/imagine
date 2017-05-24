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

#ifndef IG_MATH_ROW_H
#define IG_MATH_ROW_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename Xpr>
struct mat_traits< mat_row<Xpr> > : mat_traits<Xpr> {
  using type = mat_t<Xpr>;
  static constexpr auto n_rows = 1, 
                        n_cols = mat_traits<Xpr>::n_cols;
};

template <typename Xpr>
class mat_row : public matrix_base< mat_row<Xpr> > {
public:
  explicit mat_row(Xpr& xpr, size_t row)
    : xpr_{xpr}
    , row_{row} {}

  auto rows() const { return 1; }
  auto cols() const { return xpr_.cols(); }

  auto operator()(size_t, size_t col) const { return xpr_(row_, col); }
  auto& operator()(size_t, size_t col)      { return xpr_(row_, col); }

  auto operator[](size_t n) const { return xpr_(row_, n); }
  auto& operator[](size_t n)      { return xpr_(row_, n); }

  auto operator=(const mat_row& o) {
    eval(*this, o); return *this;
  }

  template <typename Mat>
  auto operator=(const matrix_base<Mat>& o) {
    eval(*this, o); return *this;
  }

private:
  Xpr& xpr_;
  size_t row_;
};

} // namespace ig

#endif // IG_MATH_ROW_H
