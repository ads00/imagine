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

#ifndef IG_MATH_MATRIX_H
#define IG_MATH_MATRIX_H

#include "imagine/math/theory/detail/matrix/base.h"
#include "imagine/math/theory/detail/matrix/block.h"
#include "imagine/math/theory/detail/matrix/col.h"
#include "imagine/math/theory/detail/matrix/row.h"
#include "imagine/math/theory/detail/matrix/diag.h"

#include "imagine/math/theory/detail/matrix/type/symm.h"
#include "imagine/math/theory/detail/matrix/type/triang.h"

#include "imagine/math/theory/detail/matrix/expr/unary.h"
#include "imagine/math/theory/detail/matrix/expr/binary.h"
#include "imagine/math/theory/detail/matrix/expr/prod.h"
#include "imagine/math/theory/detail/matrix/expr/trans.h"
#include "imagine/math/theory/detail/matrix/expr/relational.h"

namespace ig {

template
< typename T,
  size_t M,
  size_t N >
struct matrix_traits
<
  matrix<T, M, N>
>
{
  using value_type = T;
  static constexpr auto n_rows = M, n_cols = N;
};

template
< typename t_,
  size_t m_ = dynamic_size,
  size_t n_ = m_ >
class matrix : public matrix_base< matrix<t_, m_, n_> > {
public:
  using value_type = t_;
  static constexpr auto dynamic_rows = !m_;
  static constexpr auto dynamic_cols = !n_;
  static constexpr auto dynamic = (dynamic_rows && dynamic_cols);
  static constexpr auto hybrid  = (dynamic_rows || dynamic_cols);
  static constexpr auto immutable = !hybrid;

  template < bool X = immutable, typename = std::enable_if_t<X> >
  constexpr matrix() : data_{} {}

  template < bool X = immutable, typename = std::enable_if_t<X>, typename... Args >
  constexpr explicit matrix(value_type i, Args&&... args) : data_{{i, std::forward<Args>(args)...}} {
    [this](size_t s)
    { std::fill(data_.d.begin() + s + 1, data_.d.end(), data_.d[s]); }(sizeof...(args));
  }

  template < bool X = hybrid, typename = std::enable_if_t<X> >
  constexpr explicit matrix(size_t n)
    : data_{
        {},
        dynamic_rows ? n : static_cast<size_t>(m_),
        dynamic_cols ? n : static_cast<size_t>(n_)} { data_.d.resize(data_.m * data_.n); }

  template < bool X = dynamic, typename = std::enable_if_t<X> >
  constexpr explicit matrix(size_t m, size_t n)
    : data_{
        container_type(m * n),
        m,
        n} {}

  template <typename Mat>
  matrix(const matrix_base<Mat>& o) : matrix{o, std::integral_constant<bool, immutable>{}}
  { eval(*this, o, data_); }

  template <typename Mat> matrix(const matrix_base<Mat>& o, std::true_type) {}
  template <typename Mat> matrix(const matrix_base<Mat>& o, std::false_type)
    : data_{
        container_type(
          o.rows() * o.cols()),
          o.rows(),  o.cols()} {}

  matrix(const matrix& o) : data_{o.derived().data_} {}

  auto rows() const { return data_.rows_impl(); }
  auto cols() const { return data_.cols_impl(); }

  auto buffer() const { return data_.d.data(); }
  auto buffer()       { return data_.d.data(); }

  auto operator()(size_t row, size_t col) const -> const value_type&;
  auto operator()(size_t row, size_t col) -> value_type&;

  auto operator[](size_t n) const -> const value_type&;
  auto operator[](size_t n) -> value_type&;

  auto make_eye() -> matrix&;

  template < bool X = immutable, typename = std::enable_if_t<X> >
  static auto eye()         { return matrix{ }.make_eye(); }

  template < bool X = hybrid,    typename = std::enable_if_t<X> >
  static auto eye(size_t n) { return matrix{n}.make_eye(); }

private:
  using container_type = std::conditional_t
    < hybrid,
      std::vector<value_type>,
      std::array <value_type, m_ * n_>
    >;

  struct dynamic_data {
    size_t rows_impl() const
    { return m; }
    size_t cols_impl() const
    { return n; } container_type d; size_t m, n; };
  struct static_data {
    size_t rows_impl() const
    { return m_; }
    size_t cols_impl() const
    { return n_; } container_type d; };

  std::conditional_t
  < hybrid,
    dynamic_data, static_data
  > data_;
};

template
< typename t_,
  size_t m_,
  size_t n_ >
auto matrix<t_, m_, n_>::operator()(size_t row, size_t col) const -> const value_type& {
  assert(
    row < rows() &&
    col < cols()
    && "Invalid matrix subscript");
  return data_.d[row * cols() + col];
}

template
< typename t_,
  size_t m_,
  size_t n_ >
auto matrix<t_, m_, n_>::operator()(size_t row, size_t col) -> value_type& {
  assert(
    row < rows() &&
    col < cols()
    && "Invalid matrix subscript");
  return data_.d[row * cols() + col];
}

template
< typename t_,
  size_t m_,
  size_t n_ >
auto matrix<t_, m_, n_>::operator[](size_t n) const -> const value_type& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d[n];
}

template
< typename t_,
  size_t m_,
  size_t n_ >
auto matrix<t_, m_, n_>::operator[](size_t n) -> value_type& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d[n];
}

// Eye (Identity)
template
< typename t_,
  size_t m_,
  size_t n_ >
auto matrix<t_, m_, n_>::make_eye() -> matrix& {
  size_t i = 0, stride = matrix::diag_size() + 1;
  std::generate(matrix::begin(), matrix::end(), [this, &i, &stride]() { return !(i++ % stride); });
  return *this;
}

} // namespace ig

#endif // IG_MATH_MATRIX_H
