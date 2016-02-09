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

#ifndef COMPUTE_ALG_BLOCK_H
#define COMPUTE_ALG_BLOCK_H

#include "imagine/compute/linalg/base/alg.h"

namespace ig
{

template <typename TXpr>
struct alg_traits< alg_block<TXpr> > : alg_traits<TXpr>
{
  using T = alg_t<TXpr>;
  static constexpr int M = TXpr::M;
  static constexpr int N = TXpr::N;
};

template <typename TXpr>
class alg_block : public alg< alg_block<TXpr> >
{
public:
  constexpr alg_block(TXpr& xpr, std::size_t row, std::size_t col, std::size_t nr, std::size_t nc)
    : xpr_{xpr}, colvec_{false}, rowvec_{false}, row_{row}, col_{col}, nr_{nr}, nc_{nc} {}

  constexpr alg_block(TXpr& xpr, std::size_t start, std::size_t n)
    : xpr_{xpr}, colvec_{xpr_.cols() == 1}, rowvec_{xpr_.rows() == 1},
      row_{colvec_ ? start : 0}, col_{rowvec_ ? start : 0},
      nr_{colvec_ ? n : 1},      nc_{rowvec_ ? n : 1} {}

  constexpr std::size_t rows() const { return nr_; }
  constexpr std::size_t cols() const { return nc_; }

  auto operator()(std::size_t row, std::size_t col) const { return xpr_(row_ + row, col_ + col); }
  auto& operator()(std::size_t row, std::size_t col)      { return xpr_(row_ + row, col_ + col); }

  auto operator[](std::size_t n) const = delete;
  auto& operator[](std::size_t n)      = delete;

  alg_block& operator=(const alg_block& o)
  {
    eval(*this, o);
    return *this;
  }

  template <typename TAlg>
  alg_block& operator=(const alg<TAlg>& o)
  {
    eval(*this, o);
    return *this;
  }

private:
  TXpr& xpr_;
  bool colvec_, rowvec_;
  std::size_t row_, col_, nr_, nc_;
};

} // namespace ig

#endif // COMPUTE_ALG_BLOCK_H
