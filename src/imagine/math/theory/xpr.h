/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_XPR_H
#define IG_MATH_XPR_H

#include "imagine/ig.h"

namespace ig {

template <typename Derived>
class xpr {
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

#endif // IG_MATH_XPR_H
