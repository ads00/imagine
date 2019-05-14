/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_XPR_H
#define IG_MATH_XPR_H

#include "imagine/core/mt/tag.h"
#include "imagine/core/mt/reverse.h"

namespace ig {

template <typename D>
class xpr {
public:
  auto& derived() const { return static_cast<const D&>(*this); }
  auto& derived()       { return static_cast<      D&>(*this); }

  auto size() const { return derived().size(); }

  auto begin() const { return iterator<const D>{derived(), 0}; }
  auto begin()       { return iterator<      D>{derived(), 0}; }
  auto end() const   { return iterator<const D>{derived(), size()}; }
  auto end()         { return iterator<      D>{derived(), size()}; }

  auto ndbegin() const { return nditerator<const D>{derived(), 0}; }
  auto ndbegin()       { return nditerator<      D>{derived(), 0}; }
  auto ndend() const   { return nditerator<const D>{derived(), size()}; }
  auto ndend()         { return nditerator<      D>{derived(), size()}; }

  template <typename Xpr> class iterator;
  template <typename Xpr> class nditerator;
};

template <typename D>
template <typename Xpr>
class xpr<D>::iterator {
public:
  using value_type = typename D::value_type;
  using pointer    = value_type*;
  using reference  = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  explicit iterator(Xpr& xpr, size_t i)
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
  auto operator*()       -> decltype(auto)
  { return xpr_[i_]; }

protected:
  size_t i_;
  Xpr& xpr_;
};

template <typename D>
template <typename Xpr>
class xpr<D>::nditerator {
public:
  using value_type = typename D::value_type;
  using pointer    = value_type*;
  using reference  = value_type&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  explicit constexpr nditerator(Xpr& xpr, size_t i)
    : i_{i}
    , xpr_{xpr} {}

  auto& operator++() { ++i_; return *this; }
  auto& operator--() { --i_; return *this; }

  auto& operator+=(size_t o) { i_ += o; return *this; }
  auto& operator-=(size_t o) { i_ -= o; return *this; }
  auto& operator= (const nditerator& o) { i_ = o.i_; return *this; }

  auto operator+(const nditerator& o)
  { return i_ + o.i_; }
  auto operator-(const nditerator& o)
  { return i_ - o.i_; }

  bool operator==(const nditerator& o)
  { return i_ == o.i_; }
  bool operator!=(const nditerator& o)
  { return i_ != o.i_; }
  bool operator<(const nditerator& o)
  { return i_ < o.i_; }

  auto operator*() const -> decltype(auto)
  { return xpr_.eval(i_); }
  auto operator*()       -> decltype(auto)
  { return xpr_.eval(i_); }

protected:
  size_t i_;
  Xpr& xpr_;
};

} // namespace ig

#endif // IG_MATH_XPR_H
