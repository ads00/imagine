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

#ifndef IG_MATH_EXPRESSION_H
#define IG_MATH_EXPRESSION_H

#include "imagine/ig.h"

namespace ig {

template <typename Derived>
class expression {
public:
  auto& derived() const { return static_cast<const Derived&>(*this); }
  auto& derived()       { return static_cast<      Derived&>(*this); }

  auto eval() const
  { return derived().eval(); }

  template <typename Expr>
  class iterator {
  public:
    explicit iterator(Expr& xpr, size_t i)
      : i_{i}
      , xpr_{xpr} {}

    auto& operator++() { ++i_; return *this; }
    auto& operator--() { --i_; return *this; }
    auto& operator=(const iterator& o) { i_ = o.i_; return *this; }

    auto operator+(const iterator& o)
    { return i_ + o.i_; }
    auto operator-(const iterator& o)
    { return i_ - o.i_; }

    bool operator==(const iterator& o)
    { return i_ == o.i_; }
    bool operator!=(const iterator& o)
    { return i_ != o.i_; }
    bool operator<(const iterator& o)
    { return i_ < o.i_; }

    auto operator*() const -> decltype(auto)
    { return xpr_[i_]; }

  protected:
    size_t i_;
    Expr& xpr_;
  };
};

} // namespace ig

#endif // IG_MATH_EXPRESSION_H
