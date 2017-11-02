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

#ifndef IG_MATH_MATRIXBASE_H
#define IG_MATH_MATRIXBASE_H

#include "imagine/math/basis.h"

#include <vector>
#include <array>

namespace ig {

constexpr size_t dynamic_size = 0;
template 
< typename T, 
  size_t M,
  size_t N > class matrix;
template 
< typename T, 
  size_t N = dynamic_size > using colvec = matrix<T, N, 1>;
template 
< typename T, 
  size_t N = dynamic_size > using rowvec = matrix<T, 1, N>;

template 
< typename Mat,
  typename FunctionObject > class matrix_unary;
template
< typename Lhs,
  typename Rhs,
  typename FunctionObject > class matrix_binary;
template 
< typename Lhs, 
  typename Rhs > class matrix_product;

template <typename Xpr> class matrix_col;
template <typename Xpr> class matrix_row;
template <typename Xpr> class matrix_diag;
template <typename Mat> class matrix_trans;
template <typename Xpr> class matrix_block;

template <typename Mat> class matrix_symm;
template <typename Mat> class matrix_triang;

// Meta
template <typename Xpr> struct matrix_traits;
template <typename Xpr> struct matrix_traits<const Xpr> : matrix_traits<Xpr> {};

// Aliases
template <typename Mat> using matrix_t = typename matrix_traits<Mat>::value_type;
template <typename Mat> using concrete_matrix = 
  matrix
  <
    matrix_t<Mat>,
    matrix_traits<Mat>::n_rows, matrix_traits<Mat>::n_cols
  >;

template <typename Derived>
class matrix_base {
public:
  using value_type = matrix_t<Derived>;

  auto& derived() const { return static_cast<const Derived&>(*this); }
  auto& derived()       { return static_cast<Derived&>(*this); }

  template <typename ConstDerived>
  class iterator {
  public:
    explicit iterator(ConstDerived& derived, size_t i)
      : i_{i}
      , derived_{derived} {}

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
    { return derived_[i_]; }

  protected:
    size_t i_;
    ConstDerived& derived_;
  };

  auto begin() const { return iterator<const Derived>{derived(), 0}; }
  auto begin()       { return iterator<Derived>{derived(), 0}; }
  auto end() const   { return iterator<const Derived>{derived(), size()}; }
  auto end()         { return iterator<Derived>{derived(), size()}; }

  auto square() const { return rows() == cols(); }
  auto vector() const { return rows() == 1 || cols() == 1; }

  auto rows() const { return derived().rows(); }
  auto cols() const { return derived().cols(); }
  auto size() const { return rows() * cols(); }

  auto diagsize() const { return std::min(rows(), cols()); }
  auto vecsize() const  { return std::max(rows(), cols()); }

  auto col(size_t n) const { return matrix_col<const Derived>{derived(), n}; }
  auto col(size_t n)       { return matrix_col<Derived>{derived(), n}; }
  auto row(size_t n) const { return matrix_row<const Derived>{derived(), n}; }
  auto row(size_t n)       { return matrix_row<Derived>{derived(), n}; }

  auto diag() const { return matrix_diag<const Derived>{derived()}; }
  auto diag()       { return matrix_diag<Derived>{derived()}; }
  auto t() const    { return matrix_trans<const Derived>{derived()}; }
  auto t()          { return matrix_trans<Derived>{derived()}; }

  auto head(size_t n) const { return matrix_block<const Derived>{derived(), 0, n}; }
  auto head(size_t n)       { return matrix_block<Derived>{derived(), 0, n}; }
  auto tail(size_t n) const { return matrix_block<const Derived>{derived(), size() - n, n}; }
  auto tail(size_t n)       { return matrix_block<Derived>{derived(), size() - n, n}; }

  decltype(auto) operator()(size_t row, size_t col) const
  { return derived()(row, col); }
  decltype(auto) operator()(size_t row, size_t col) 
  { return derived()(row, col); }

  decltype(auto) operator[](size_t index) const 
  { return derived()[index]; }
  decltype(auto) operator[](size_t index)
  { return derived()[index]; }

  auto& operator+=(value_type value) 
  { return derived() = std::move(*this) + value; }
  auto& operator-=(value_type value)
  { return derived() = std::move(*this) - value; }
  auto& operator*=(value_type value)
  { return derived() = std::move(*this) * value; }
  auto& operator/=(value_type value)
  { return derived() = std::move(*this) / value; }

  template <typename Mat> 
  auto& operator+=(const matrix_base<Mat>& mat) { return derived() = std::move(*this) + mat; }
  template <typename Mat> 
  auto& operator-=(const matrix_base<Mat>& mat) { return derived() = std::move(*this) - mat; }
  template <typename Mat> 
  auto& operator/=(const matrix_base<Mat>& mat) { return derived() = std::move(*this) / mat; }
  template <typename Mat>
  auto& operator%=(const matrix_base<Mat>& mat) { return derived() = std::move(*this) % mat; }

  template <typename Mat> 
  auto& operator*=(const matrix_base<Mat>& mat) {
    assert(Derived::hybrid && "Cannot multiply in place an immutable matrix");
    return derived() = std::move(*this) * mat;
  }

  auto sum() const  -> typename value_type;
  auto prod() const -> typename value_type;
  auto mean() const -> typename value_type;

  class initializer {
  public:
    explicit initializer(Derived& mat)
      : mat_{mat}
      , row_{0}
      , col_{0}
      , curr_{0} {}

    auto operator,(value_type value) {
      if (col_ == mat_.cols()) 
        row_++, 
        col_ = 0;
      mat_(row_, col_++) = value;
      curr_++; 
      return *this;
    }

  private:
    Derived& mat_;
    size_t row_, col_, curr_;
  };

  auto operator<<(value_type val) 
  { return initializer{derived()}, val; }
};

template <typename Gen, typename Mat>
void eval_helper(matrix_base<Gen>& ev, const matrix_base<Mat>& mat) {
  assert(
    ev.rows() == mat.rows() && 
    ev.cols() == mat.cols()
    && "Incoherent algebraic evaluation");

  for (size_t i = 0; i < ev.rows(); ++i)
    for (size_t j = 0; j < ev.cols(); ++j) 
      ev (i, j) = 
      mat(i, j);
}

template <typename Gen, typename Mat>
auto eval(matrix_base<Gen>& ev, const matrix_base<Mat>& mat)
{ eval_helper(ev, mat); return ev; }

template <typename Gen, typename Mat, typename O>
void eval(matrix_base<Gen>& ev, const matrix_base<Mat>& mat, const O&)
{ eval_helper(ev, mat); }

template <typename Gen, typename Mat>
void eval(matrix_base<Gen>& ev, const matrix_base<Mat>& mat, typename Gen::dynamics_data& i) {
  i.d_.resize(ev.size());
  eval_helper(ev, mat);
}

// Cwise
template <typename Derived>
auto matrix_base<Derived>::sum() const -> typename value_type
{ return std::accumulate(begin(), end(), value_type(0)); }

template <typename Derived>
auto matrix_base<Derived>::prod() const -> typename value_type
{ return std::accumulate(begin(), end(), value_type(1), std::multiplies<>{}); }

template <typename Derived>
auto matrix_base<Derived>::mean() const -> typename value_type
{ return sum() / size(); }

template <typename Mat>
inline std::ostream& operator<<(std::ostream& stream, const matrix_base<Mat>& mat) {
  size_t width = 0;
  std::stringstream w{}; w.precision(5);

  for (auto elemt : mat) {
    w.str(std::string{});
    w.clear();
    w << std::fixed << elemt; width = std::max<size_t>(width, size_t(w.tellp()));
  }

  stream.precision(5);
  stream << std::fixed;
  for (size_t i = 0; i < mat.rows(); ++i) {
    stream << std::endl; stream.width(width); stream << mat(i, 0);
    for (size_t j = 1; j < mat.cols(); ++j) {
      stream << ' ';     stream.width(width); stream << mat(i, j);
    }
  } return stream;
}

} // namespace ig

#endif // IG_MATH_MATRIXBASE_H
