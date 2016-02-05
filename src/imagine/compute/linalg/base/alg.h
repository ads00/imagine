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

#ifndef COMPUTE_ALG_H
#define COMPUTE_ALG_H

#include "imagine/compute/math.h"
#include <vector>
#include <array>

namespace ig
{

constexpr int dynamic_sized = -1;
template <typename T, int M, int N> class matrix;
template <typename T, int N = dynamic_sized> using vector = matrix<T, N, 1>;
template <typename T, int N = dynamic_sized> using row    = matrix<T, 1, N>;

template <typename TXpr> class alg_block;
template <typename TXpr> class alg_col;
template <typename TXpr> class alg_row;
template <typename TXpr> class alg_diag;
template <typename TAlg> class alg_trans;
template <typename TAlg> class alg_triang;

template <typename TAlg, typename TOp> class unary_expr;
template <typename TAlg, typename TOp> class scalar_expr;
template <typename TLhs, typename TRhs> class product_expr;
template <typename TLhs, typename TRhs, typename TOp> class binary_expr;

// meta
template <typename TXpr> struct alg_traits;
template <typename TXpr> struct alg_traits<const TXpr> : alg_traits<TXpr>{};

template <typename TAlg> 
using alg_t = typename TAlg::T;

template <typename Derived>
class alg
{
public:
  using T = typename alg_traits<Derived>::T;
  static constexpr int M = alg_traits<Derived>::M;
  static constexpr int N = alg_traits<Derived>::N;

  using plain_t = matrix<T, M, N>;
  using U = std::conditional_t<std::is_same<Derived, plain_t>::value, const T&, T>;

  const Derived& derived() const { return static_cast<const Derived&>(*this); }
  Derived& derived()             { return static_cast<Derived&>(*this); }

  template <typename TIt, typename TDer>
  class iterator : public std::iterator<std::random_access_iterator_tag, T>
  {
  public:
    friend alg;

    iterator& operator=(const iterator& o) { pos_ = o.pos_; return *this; }

    const iterator& operator++() { ++pos_; return *this; }
    const iterator& operator--() { --pos_; return *this; }

    iterator operator++(int) { iterator tmp(*this); ++pos_; return tmp; }
    iterator operator--(int) { iterator tmp(*this); --pos_; return tmp; }

    bool operator==(const iterator& o) { return pos_ == o.pos_; }
    bool operator!=(const iterator& o) { return pos_ != o.pos_; }

    TIt operator*() const { return derived_[pos_]; }

  private:
    constexpr iterator(TDer& derived, std::size_t pos)
      : derived_{derived}, pos_{pos} {}

    std::size_t pos_;
    TDer& derived_;
  };

  const auto begin() const { return iterator<U, const Derived>{derived(), 0}; }
  auto begin()             { return iterator<T&, Derived>{derived(), 0}; }
  const auto end() const   { return iterator<U, const Derived>{derived(), size()}; }
  auto end()               { return iterator<T&, Derived>{derived(), size()}; }

  T sum() const;
  T prod() const;
  T mean() const;

  constexpr bool square() const { return rows() == cols(); }
  constexpr bool vector() const { return rows() == 1 || cols() == 1; }

  constexpr std::size_t rows() const { return derived().rows(); }
  constexpr std::size_t cols() const { return derived().cols(); }
  constexpr std::size_t size() const { return rows() * cols(); }

  constexpr std::size_t diagsize() const { return std::min(rows(), cols()); }
  constexpr std::size_t vecsize() const  { return std::max(rows(), cols()); }

  const auto head(std::size_t n) const { return alg_block<const Derived>{derived(), 0, n}; }
  auto head(std::size_t n)             { return alg_block<Derived>{derived(), 0, n}; }
  const auto tail(std::size_t n) const { return alg_block<const Derived>{derived(), size() - n, n}; }
  auto tail(std::size_t n)             { return alg_block<Derived>{derived(), size() - n, n}; }

  const auto row(std::size_t n) const { return alg_row<const Derived>{derived(), n}; }
  auto row(std::size_t n)             { return alg_row<Derived>{derived(), n}; }
  const auto col(std::size_t n) const { return alg_col<const Derived>{derived(), n}; }
  auto col(std::size_t n)             { return alg_col<Derived>{derived(), n}; }

  const auto diag() const { return alg_diag<const Derived>{derived()}; }
  auto diag()             { return alg_diag<Derived>{derived()}; }

  const auto t() const { return alg_trans<const Derived>{derived()}; }
  auto t()             { return alg_trans<Derived>{derived()}; }

  U operator()(std::size_t row, std::size_t col) const { return derived()(row, col); }
  T& operator()(std::size_t row, std::size_t col)      { return derived()(row, col); }

  U operator[](std::size_t index) const { return derived()[index]; }
  T& operator[](std::size_t index)      { return derived()[index]; }

  Derived& operator+=(T scalar) { return derived() = std::move(*this) + scalar; }
  Derived& operator-=(T scalar) { return derived() = std::move(*this) - scalar; }
  Derived& operator/=(T scalar) { return derived() = std::move(*this) / scalar; }
  Derived& operator*=(T scalar) { return derived() = std::move(*this) * scalar; }

  template <typename TAlg> 
  Derived& operator+=(const alg<TAlg>& alg) { return derived() = std::move(*this) + alg; }

  template <typename TAlg> 
  Derived& operator-=(const alg<TAlg>& alg) { return derived() = std::move(*this) - alg; }

  template <typename TAlg> 
  Derived& operator/=(const alg<TAlg>& alg) { return derived() = std::move(*this) / alg; }

  template <typename TAlg>
  Derived& operator%=(const alg<TAlg>& alg) { return derived() = std::move(*this) % alg; }

  template <typename TAlg> 
  Derived& operator*=(const alg<TAlg>& alg)
  {
    assert(Derived::hybrids && "Cannot multiply in place a static matrix");
    return derived() = std::move(*this) * alg;
  }

  class alg_initializer
  {
  public:
    alg_initializer(Derived& alg, T val)
      : alg_{alg}, row_{0}, col_{1}, curr_{1}
    {
      alg_(0, 0) = val;
    }

    alg_initializer& operator,(T val)
    {
      if (col_ == alg_.cols())
        row_++, col_ = 0;

      alg_(row_, col_++) = val;
      return *this;
    }

  private:
    Derived& alg_;
    std::size_t row_, col_, curr_;
  };

  alg_initializer operator<<(T val)
  {
    return alg_initializer{derived(), val};
  }
};

template <typename TEval, typename TAlg>
void eval_helper(alg<TEval>& ev, const alg<TAlg>& alg)
{
  assert(ev.rows() == alg.rows() && ev.cols() == alg.cols()
         && "Incoherent algebraic evaluation");

  const std::size_t evc = ev.cols(),
                    evr = ev.rows();

  for (std::size_t i = 0; i < evc; ++i)
    for (std::size_t j = 0; j < evr; ++j) ev(j, i) = alg(j, i);
}

template <typename TEval, typename TAlg>
constexpr void eval(alg<TEval>& ev, const alg<TAlg>& alg)
{
  eval_helper(ev, alg);
}

template <typename TEval, typename TAlg, typename TO>
constexpr void eval(alg<TEval>& ev, const alg<TAlg>& alg, const TO& i)
{
  eval_helper(ev, alg);
}

template <typename TEval, typename TAlg>
constexpr void eval(alg<TEval>& ev, const alg<TAlg>& alg, std::vector< alg_t<TEval> >& i)
{
  i.resize(ev.size());
  eval_helper(ev, alg);
}

// cwise
template <typename Derived>
auto alg<Derived>::sum() const -> T
{
  return std::accumulate(begin(), end(), T(0));
}

template <typename Derived>
auto alg<Derived>::prod() const -> T
{
  return std::accumulate(begin(), end(), T(1),
                         std::multiplies<T>{});
}

template <typename Derived>
auto alg<Derived>::mean() const -> T
{
  return sum() / size();
}

template <typename TAlg>
inline std::ostream& operator<<(std::ostream& stream, const alg<TAlg>& alg)
{
  std::size_t width = 0;
  std::stringstream w; w.precision(3);
  for (auto v : alg)
  {
    w.str(std::string{}); w.clear(); w << std::fixed << v;
    width = std::max<std::size_t>(width, std::size_t(w.tellp()));
  }

  stream.precision(3);
  stream.setf(std::ios::fixed);
  for (std::size_t i = 0; i < alg.rows(); ++i)
  {
    stream << std::endl;
    stream.width(width); stream << alg(i, 0);
    for (std::size_t j = 1; j < alg.cols(); ++j)
    {
      stream << ' ';
      stream.width(width); stream << alg(i, j);
    }
  }

  return stream;
}

} // namespace ig

#endif // COMPUTE_ALG_H
