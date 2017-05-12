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

#ifndef IG_MATH_MATRIX_BASE_H
#define IG_MATH_MATRIX_BASE_H

#include "imagine/math/basis.h"

#include <vector>
#include <array>
#include <numeric>
#include <algorithm>

namespace ig {

constexpr int32_t dynamic_sized = -1;
template <typename T, int32_t M, int32_t N> class matrix;
template <typename T, int32_t N = dynamic_sized> using colvec = matrix<T, N, 1>;
template <typename T, int32_t N = dynamic_sized> using rowvec = matrix<T, 1, N>;

template <typename Xpr> class mat_block;
template <typename Xpr> class mat_col;
template <typename Xpr> class mat_row;
template <typename Xpr> class mat_diag;
template <typename Mat> class mat_trans;

template <typename Mat> class mat_symm;
template <typename Mat> class mat_triang;

template <typename Mat, typename Op> class unary_expr;
template <typename Lhs, typename Rhs> class product_expr;
template <typename Lhs, typename Rhs, typename Op> class binary_expr;

// Meta
template <typename Xpr> struct mat_traits;
template <typename Xpr> struct mat_traits<const Xpr> : mat_traits<Xpr> {};

// _t alias like
template <typename Mat> using mat_t = typename Mat::type;

template <typename C>
class matrix_base {
public:
  using type = typename mat_traits<C>::type;
  static constexpr auto n_rows = mat_traits<C>::n_rows;
  static constexpr auto n_cols = mat_traits<C>::n_cols;

  using plain_type = matrix<type, n_rows, n_cols>;
  using cdti_type = std::conditional_t<std::is_same<C, plain_type>::value, const type&, type>;

  auto& derived() const { return static_cast<const C&>(*this); }
  auto& derived()       { return static_cast<C&>(*this); }

  template <typename iter, typename citer>
  class iterator : public std::iterator<std::random_access_iterator_tag, type> {
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

  auto begin() const { return iterator<cdti_type, const C>{derived(), 0}; }
  auto begin()       { return iterator<type&, C>{derived(), 0}; }
  auto end() const   { return iterator<cdti_type, const C>{derived(), size()}; }
  auto end()         { return iterator<type&, C>{derived(), size()}; }

  auto sum() const -> type;
  auto prod() const -> type;
  auto mean() const -> type;

  auto square() const { return rows() == cols(); }
  auto vector() const { return rows() == 1 || cols() == 1; }

  auto rows() const { return derived().rows(); }
  auto cols() const { return derived().cols(); }
  auto size() const { return rows() * cols(); }

  auto diagsize() const { return std::min(rows(), cols()); }
  auto vecsize() const  { return std::max(rows(), cols()); }

  auto head(size_t n) const { return mat_block<const C>{derived(), 0, n}; }
  auto head(size_t n)       { return mat_block<C>{derived(), 0, n}; }
  auto tail(size_t n) const { return mat_block<const C>{derived(), size() - n, n}; }
  auto tail(size_t n)       { return mat_block<C>{derived(), size() - n, n}; }

  auto row(size_t n) const { return mat_row<const C>{derived(), n}; }
  auto row(size_t n)       { return mat_row<C>{derived(), n}; }
  auto col(size_t n) const { return mat_col<const C>{derived(), n}; }
  auto col(size_t n)       { return mat_col<C>{derived(), n}; }

  auto diag() const { return mat_diag<const C>{derived()}; }
  auto diag()       { return mat_diag<C>{derived()}; }

  auto t() const { return mat_trans<const C>{derived()}; }
  auto t()       { return mat_trans<C>{derived()}; }

  auto operator()(size_t row, size_t col) const { return derived()(row, col); }
  auto& operator()(size_t row, size_t col)      { return derived()(row, col); }

  auto operator[](size_t index) const { return derived()[index]; }
  auto& operator[](size_t index)      { return derived()[index]; }

  auto& operator+=(type scalar) { return derived() = std::move(*this) + scalar; }
  auto& operator-=(type scalar) { return derived() = std::move(*this) - scalar; }
  auto& operator/=(type scalar) { return derived() = std::move(*this) / scalar; }
  auto& operator*=(type scalar) { return derived() = std::move(*this) * scalar; }

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
    assert(C::hybrid && "Cannot multiply in place an immutable matrix");
    return derived() = std::move(*this) * mat;
  }

  class initializer {
  public:
    explicit initializer(C& mat) 
      : mat_{mat}
      , row_{0}
      , col_{0}
      , curr_{0} {}

    auto operator,(type val) {
      if (col_ == mat_.cols()) row_++, col_ = 0;
      mat_(row_, col_++) = val;
      curr_++; return *this;
    }

  private:
    C& mat_;
    size_t row_, col_, curr_;
  };

  auto operator<<(type val) { return initializer{derived()}, val; }
};

template <typename gen, typename Mat>
void eval_helper(matrix_base<gen>& ev, const matrix_base<Mat>& mat) {
  assert(ev.rows() == mat.rows() && ev.cols() == mat.cols()
         && "Incoherent algebraic evaluation");

  auto evr = ev.rows(), evc = ev.cols();
  for (size_t i = 0; i < evc; ++i)
    for (size_t j = 0; j < evr; ++j) ev(j, i) = mat(j, i);
}

template <typename gen, typename Mat>
void eval(matrix_base<gen>& ev, const matrix_base<Mat>& mat) {
  eval_helper(ev, mat);
}

template <typename gen, typename Mat, typename O>
void eval(matrix_base<gen>& ev, const matrix_base<Mat>& mat, const O&) {
  eval_helper(ev, mat);
}

template <typename gen, typename Mat>
void eval(matrix_base<gen>& ev, const matrix_base<Mat>& mat, typename gen::dynamics_data& i) {
  i.d_.resize(ev.size());
  eval_helper(ev, mat);
}

// Cwise
template <typename C>
auto matrix_base<C>::sum() const -> type {
  return std::accumulate(begin(), end(), type(0));
}

template <typename C>
auto matrix_base<C>::prod() const -> type {
  return std::accumulate(begin(), end(), type(1), std::multiplies<type>{});
}

template <typename C>
auto matrix_base<C>::mean() const -> type {
  return sum() / size();
}

template <typename Mat>
inline std::ostream& operator<<(std::ostream& stream, const matrix_base<Mat>& mat) {
  size_t width = 0;
  std::stringstream 
    w{}; w.precision(5);

  for (auto elemt : mat) {
    w.str(std::string{}); w.clear(); w << std::fixed << elemt;
    width = std::max<size_t>(width, size_t(w.tellp()));
  }

  stream.precision(5); stream.setf(std::ios::fixed);
  for (size_t i = 0; i < mat.rows(); ++i) {
    stream << std::endl;
    stream.width(width); stream << mat(i, 0);
    for (size_t j = 1; j < mat.cols(); ++j) {
      stream << ' ';
      stream.width(width); stream << mat(i, j);
    }
  }
  return stream;
}

} // namespace ig

#endif // IG_MATH_MATRIX_BASE_H
