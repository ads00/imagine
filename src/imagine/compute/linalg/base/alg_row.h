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

#ifndef COMPUTE_ALG_ROW_H
#define COMPUTE_ALG_ROW_H

#include "imagine/compute/linalg/base/alg.h"

namespace ig
{

template <typename Xpr>
struct alg_traits< alg_row<Xpr> > : alg_traits<Xpr>
{
  using T = alg_t<Xpr>;
  static constexpr int M = 1;
  static constexpr int N = Xpr::N;
};

template <typename Xpr>
class alg_row : public alg< alg_row<Xpr> >
{
public:
  constexpr alg_row(Xpr& xpr, std::size_t row)
    : xpr_{xpr}, row_{row} {}

  constexpr std::size_t rows() const { return 1; }
  constexpr std::size_t cols() const { return xpr_.cols(); }

  auto operator()(std::size_t, std::size_t col) const { return xpr_(row_, col); }
  auto& operator()(std::size_t, std::size_t col)      { return xpr_(row_, col); }

  auto operator[](std::size_t n) const { return xpr_(row_, n); }
  auto& operator[](std::size_t n)      { return xpr_(row_, n); }

  alg_row& operator=(const alg_row& o)
  {
    eval(*this, o);
    return *this;
  }

  template <typename Alg>
  alg_row& operator=(const alg<Alg>& o)
  {
    eval(*this, o);
    return *this;
  }

private:
  Xpr& xpr_;
  std::size_t row_;
};

} // namespace ig

#endif // COMPUTE_ALG_ROW_H
