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

#ifndef COMPUTE_ALG_DIAG_H
#define COMPUTE_ALG_DIAG_H

#include "imagine/compute/linalg/base/alg.h"

namespace ig
{

template <typename TXpr>
struct alg_traits< alg_diag<TXpr> > : alg_traits<TXpr>
{
  using T = alg_t<TXpr>;
  static constexpr int M = TXpr::M;
  static constexpr int N = 1;
};

template <typename TXpr>
class alg_diag : public alg< alg_diag<TXpr> >
{
public:
  alg_diag(TXpr& xpr)
    : xpr_{xpr} {}

  constexpr std::size_t rows() const { return xpr_.diagsize(); }
  constexpr std::size_t cols() const { return 1; }

  U operator()(std::size_t row, std::size_t) const { return xpr_(row, row); }
  T& operator()(std::size_t row, std::size_t)      { return xpr_(row, row); }

  U operator[](std::size_t n) const { return xpr_(n, n); }
  T& operator[](std::size_t n)      { return xpr_(n, n); }

  alg_diag& operator=(const alg_diag& o)
  {
    eval(*this, o);
    return *this;
  }

  template <typename TAlg>
  alg_diag& operator=(const alg<TAlg>& o)
  {
    eval(*this, o);
    return *this;
  }

private:
  TXpr& xpr_;
};

} // namespace ig

#endif // COMPUTE_ALG_DIAG_H
