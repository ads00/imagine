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

#include "imagine/math/discrete.h"

#include <vector>
#include <array>
#include <numeric>
#include <algorithm>

namespace ig {

constexpr int32_t dynamic_sized = -1;
template <typename T, int32_t M, int32_t N> class matrix;
template <typename T, int32_t N = dynamic_sized> using colvec = matrix<T, N, 1>;
template <typename T, int32_t N = dynamic_sized> using rowvec = matrix<T, 1, N>;

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
template <typename Xpr> struct alg_traits<const Xpr> : alg_traits<Xpr> {};

// _t alias like
template <typename Alg> using alg_t = typename Alg::T;

template <typename C>
class alg {
public:
  using T = typename alg_traits<C>::T;
  static constexpr auto M = alg_traits<C>::M;
  static constexpr auto N = alg_traits<C>::N;

  using plain_type = matrix<T, M, N>;
  using U = std::conditional_t<std::is_same<C, plain_type>::value, const T&, T>;

  auto& derived() const { return static_cast<const C&>(*this); }
  auto& derived()       { return static_cast<C&>(*this); }

  template <typename iter, typename citer>
  class iterator : public std::iterator<std::random_access_iterator_tag, T> {
  public:
    explicit iterator(citer& derived, size_t i) 
      : i_{i}
      , derived_{derived} {}

    auto operator=(const iterator& o) { i_ = o.i_; return *this; }
    auto operator++() { ++i_; return *this; }
    auto operator--() { --i_; return *this; }

    bool operator==(const iterator& o) { return i_ == o.i_; }
    bool operator!=(const iterator& o) { return i_ != o.i_; }
    bool operator<(const iterator& o) { return i_ < o.i_; }

    iter operator*() const { return derived_[i_]; }

  protected:
    size_t i_; citer& derived_;
  };

  auto begin() const { return iterator<U, const C>{derived(), 0}; }
  auto begin()       { return iterator<T&, C>{derived(), 0}; }
  auto end() const   { return iterator<U, const C>{derived(), size()}; }
  auto end()         { return iterator<T&, C>{derived(), size()}; }

  auto sum() const -> T;
  auto prod() const -> T;
  auto mean() const -> T;

  auto square() const { return rows() == cols(); }
  auto vector() const { return rows() == 1 || cols() == 1; }

  auto rows() const { return derived().rows(); }
  auto cols() const { return derived().cols(); }
  auto size() const { return rows() * cols(); }

  auto diagsize() const { return std::min(rows(), cols()); }
  auto vecsize() const  { return std::max(rows(), cols()); }

  auto head(size_t n) const { return alg_block<const C>{derived(), 0, n}; }
  auto head(size_t n)       { return alg_block<C>{derived(), 0, n}; }
  auto tail(size_t n) const { return alg_block<const C>{derived(), size() - n, n}; }
  auto tail(size_t n)       { return alg_block<C>{derived(), size() - n, n}; }

  auto row(size_t n) const { return alg_row<const C>{derived(), n}; }
  auto row(size_t n)       { return alg_row<C>{derived(), n}; }
  auto col(size_t n) const { return alg_col<const C>{derived(), n}; }
  auto col(size_t n)       { return alg_col<C>{derived(), n}; }

  auto diag() const { return alg_diag<const C>{derived()}; }
  auto diag()       { return alg_diag<C>{derived()}; }

  auto t() const { return alg_trans<const C>{derived()}; }
  auto t()       { return alg_trans<C>{derived()}; }

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
    assert(C::hybrid && "Cannot multiply in place an immutable matrix");
    return derived() = std::move(*this) * alg;
  }

  class initializer {
  public:
    explicit initializer(C& alg) 
      : alg_{alg}
      , row_{0}
      , col_{0}
      , curr_{0} {}

    auto operator,(T val) {
      if (col_ == alg_.cols()) row_++, col_ = 0;
      alg_(row_, col_++) = val;
      curr_++; return *this;
    }

  private:
    C& alg_;
    size_t row_, col_, curr_;
  };

  auto operator<<(T val) { return initializer{derived()}, val; }
};

template <typename gen, typename Alg>
void eval_helper(alg<gen>& ev, const alg<Alg>& alg) {
  assert(ev.rows() == alg.rows() && ev.cols() == alg.cols()
         && "Incoherent algebraic evaluation");

  auto evr = ev.rows(), evc = ev.cols();
  for (size_t i = 0; i < evc; ++i)
    for (size_t j = 0; j < evr; ++j) ev(j, i) = alg(j, i);
}

template <typename gen, typename Alg>
void eval(alg<gen>& ev, const alg<Alg>& alg) {
  eval_helper(ev, alg);
}

template <typename gen, typename Alg, typename O>
void eval(alg<gen>& ev, const alg<Alg>& alg, const O&) {
  eval_helper(ev, alg);
}

template <typename gen, typename Alg>
void eval(alg<gen>& ev, const alg<Alg>& alg, typename gen::dynamics_data& i) {
  i.d_.resize(ev.size());
  eval_helper(ev, alg);
}

// Cwise
template <typename C>
auto alg<C>::sum() const -> T {
  return std::accumulate(begin(), end(), T(0));
}

template <typename C>
auto alg<C>::prod() const -> T {
  return std::accumulate(begin(), end(), T(1), std::multiplies<T>{});
}

template <typename C>
auto alg<C>::mean() const -> T {
  return sum() / size();
}

template <typename Alg>
inline std::ostream& operator<<(std::ostream& stream, const alg<Alg>& alg) {
  size_t width = 0;
  std::stringstream 
    w{}; w.precision(3);

  for (auto elemt : alg) {
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
