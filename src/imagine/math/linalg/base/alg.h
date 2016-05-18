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

#ifndef IG_MATH_ALG_H
#define IG_MATH_ALG_H

#include "imagine/math/kernel.h"

#include <numeric>
#include <vector>
#include <array>

namespace ig {

constexpr int dynamic_sized = -1;
template <typename T, int M, int N> class matrix;
template <typename T, int N = dynamic_sized> using vector = matrix<T, N, 1>;
template <typename T, int N = dynamic_sized> using row    = matrix<T, 1, N>;

template <typename Xpr> class alg_block;
template <typename Xpr> class alg_col;
template <typename Xpr> class alg_row;
template <typename Xpr> class alg_diag;
template <typename Alg> class alg_trans;
template <typename Alg> class alg_triang;

template <typename Alg, typename Op> class unary_expr;
template <typename Alg, typename Op> class scalar_expr;
template <typename Lhs, typename Rhs> class product_expr;
template <typename Lhs, typename Rhs, typename Op> class binary_expr;

// Meta
template <typename Xpr> struct alg_traits;
template <typename Xpr> struct alg_traits<const Xpr> : alg_traits<Xpr>{};

template <typename Alg> using alg_t = typename Alg::T;

template <typename Derived>
class alg {
public:
  using T = typename alg_traits<Derived>::T;
  static constexpr int M = alg_traits<Derived>::M;
  static constexpr int N = alg_traits<Derived>::N;

  using plain_t = matrix<T, M, N>;
  using U = std::conditional_t<std::is_same<Derived, plain_t>::value, const T&, T>;

  auto& derived() const { return static_cast<const Derived&>(*this); }
  auto& derived()       { return static_cast<Derived&>(*this); }

  template <typename It, typename DIt>
  class iterator : public std::iterator<std::random_access_iterator_tag, T> {
  public:
    friend alg;

    auto operator=(const iterator& o) { pos_ = o.pos_; return *this; }
    auto operator++() { ++pos_; return *this; }
    auto operator--() { --pos_; return *this; }

    auto operator++(int) { auto tmp = iterator{*this}; ++pos_; return tmp; }
    auto operator--(int) { auto tmp = iterator{*this}; --pos_; return tmp; }

    bool operator==(const iterator& o) { return pos_ == o.pos_; }
    bool operator!=(const iterator& o) { return pos_ != o.pos_; }
    bool operator<(const iterator& o) { return pos_ < o.pos_; }

    It operator*() const { return derived_[pos_]; }

  private:
    constexpr iterator(DIt& derived, size_t pos)
      : derived_{derived}, pos_{pos} {}

    size_t pos_;
    DIt& derived_;
  };

  auto begin() const { return iterator<U, const Derived>{derived(), 0}; }
  auto begin()       { return iterator<T&, Derived>{derived(), 0}; }
  auto end() const   { return iterator<U, const Derived>{derived(), size()}; }
  auto end()         { return iterator<T&, Derived>{derived(), size()}; }

  auto sum() const -> T;
  auto prod() const -> T;
  auto mean() const -> T;

  constexpr auto square() const { return rows() == cols(); }
  constexpr auto vector() const { return rows() == 1 || cols() == 1; }

  constexpr auto rows() const { return derived().rows(); }
  constexpr auto cols() const { return derived().cols(); }
  constexpr auto size() const { return rows() * cols(); }

  constexpr auto diagsize() const { return std::min(rows(), cols()); }
  constexpr auto vecsize() const  { return std::max(rows(), cols()); }

  auto head(size_t n) const { return alg_block<const Derived>{derived(), 0, n}; }
  auto head(size_t n)       { return alg_block<Derived>{derived(), 0, n}; }
  auto tail(size_t n) const { return alg_block<const Derived>{derived(), size() - n, n}; }
  auto tail(size_t n)       { return alg_block<Derived>{derived(), size() - n, n}; }

  auto row(size_t n) const { return alg_row<const Derived>{derived(), n}; }
  auto row(size_t n)       { return alg_row<Derived>{derived(), n}; }
  auto col(size_t n) const { return alg_col<const Derived>{derived(), n}; }
  auto col(size_t n)       { return alg_col<Derived>{derived(), n}; }

  auto diag() const { return alg_diag<const Derived>{derived()}; }
  auto diag()       { return alg_diag<Derived>{derived()}; }

  auto t() const { return alg_trans<const Derived>{derived()}; }
  auto t()       { return alg_trans<Derived>{derived()}; }

  auto operator()(size_t row, size_t col) const { return derived()(row, col); }
  auto& operator()(size_t row, size_t col)      { return derived()(row, col); }

  auto operator[](size_t index) const { return derived()[index]; }
  auto& operator[](size_t index)      { return derived()[index]; }

  auto& operator+=(T scalar) { return derived() = std::move(*this) + scalar; }
  auto& operator-=(T scalar) { return derived() = std::move(*this) - scalar; }
  auto& operator/=(T scalar) { return derived() = std::move(*this) / scalar; }
  auto& operator*=(T scalar) { return derived() = std::move(*this) * scalar; }

  template <typename Alg> 
  auto& operator+=(const alg<Alg>& alg) { return derived() = std::move(*this) + alg; }

  template <typename Alg> 
  auto& operator-=(const alg<Alg>& alg) { return derived() = std::move(*this) - alg; }

  template <typename Alg> 
  auto& operator/=(const alg<Alg>& alg) { return derived() = std::move(*this) / alg; }

  template <typename Alg>
  auto& operator%=(const alg<Alg>& alg) { return derived() = std::move(*this) % alg; }

  template <typename Alg> 
  auto& operator*=(const alg<Alg>& alg) {
    assert(Derived::hybrids && "Cannot multiply in place a static matrix");
    return derived() = std::move(*this) * alg;
  }

  class initializer {
  public:
    initializer(Derived& alg)
      : alg_{alg}, row_{0}, col_{0}, curr_{0} {
    }

    auto operator,(T val) {
      if (col_ == alg_.cols()) {
        row_++, col_ = 0;
      } alg_(row_, col_++) = val;
      curr_++; return *this;
    }

    template <typename... Args>
    static void auto_construct(Derived& alg, T val, Args&&... args) {
      auto init = initializer{alg};
      init.recursive_construct(val, std::forward<Args>(args)...);
    }

  private:
    template <typename... Args>
    void recursive_construct(T val, Args&&... args) {
      operator,(val);
      recursive_construct(std::forward<Args>(args)...);
    }

    void recursive_construct(T val) {
      operator,(val);
      if (curr_ < alg_.size()) {
        for (size_t i = curr_; i < alg_.size(); ++i) operator,(val);
      }
    }

    Derived& alg_;
    size_t row_, col_, curr_;
  };

  auto operator<<(T val) { return initializer{derived()}, val; }
};

template <typename Eval, typename Alg>
void eval_helper(alg<Eval>& ev, const alg<Alg>& alg) {
  assert(ev.rows() == alg.rows() && ev.cols() == alg.cols()
         && "Incoherent algebraic evaluation");

  auto evc = ev.cols(), evr = ev.rows();
  for (size_t i = 0; i < evc; ++i)
    for (size_t j = 0; j < evr; ++j) ev(j, i) = alg(j, i);
}

template <typename Eval, typename Alg>
constexpr void eval(alg<Eval>& ev, const alg<Alg>& alg) {
  eval_helper(ev, alg);
}

template <typename Eval, typename Alg, typename O>
constexpr void eval(alg<Eval>& ev, const alg<Alg>& alg, const O& i) {
  eval_helper(ev, alg);
}

template <typename Eval, typename Alg>
constexpr void eval(alg<Eval>& ev, const alg<Alg>& alg, std::vector< alg_t<Eval> >& i) {
  i.resize(ev.size());
  eval_helper(ev, alg);
}

// Cwise
template <typename Derived>
auto alg<Derived>::sum() const -> T {
  return std::accumulate(begin(), end(), T(0));
}

template <typename Derived>
auto alg<Derived>::prod() const -> T {
  return std::accumulate(begin(), end(), T(1), std::multiplies<T>{});
}

template <typename Derived>
auto alg<Derived>::mean() const -> T {
  return sum() / size();
}

template <typename Alg>
inline std::ostream& operator<<(std::ostream& stream, const alg<Alg>& alg) {
  auto width = size_t{0};
  auto w = std::stringstream{}; w.precision(3);

  for (auto&& elemt : alg) {
    w.str(std::string{}); w.clear(); w << std::fixed << elemt;
    width = std::max<size_t>(width, size_t(w.tellp()));
  }

  stream.precision(3); stream.setf(std::ios::fixed);
  for (size_t i = 0; i < alg.rows(); ++i) {
    stream << std::endl;
    stream.width(width); stream << alg(i, 0);
    for (size_t j = 1; j < alg.cols(); ++j) {
      stream << ' ';
      stream.width(width); stream << alg(i, j);
    }
  }
  return stream;
}

} // namespace ig

#endif // IG_MATH_ALG_H
