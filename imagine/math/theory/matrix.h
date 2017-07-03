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
#include "imagine/math/theory/detail/matrix/trans.h"

#include "imagine/math/theory/detail/matrix/expr/unary.h"
#include "imagine/math/theory/detail/matrix/expr/product.h"
#include "imagine/math/theory/detail/matrix/expr/binary.h"
#include "imagine/math/theory/detail/matrix/expr/relational.h"

namespace ig {

template <typename T, int32_t M, int32_t N>
struct mat_traits< matrix<T, M, N> > {
  using type = T;
  static constexpr auto n_rows = M, 
                        n_cols = N;
};

template <typename T, int32_t M = dynamic_sized, int32_t N = M>
class matrix : public matrix_base< matrix<T, M, N> > {
public:
  using type = T;
  using base_type = matrix_base< matrix<type, M, N> >;

  static constexpr auto dynamic_rows = (M < 0);
  static constexpr auto dynamic_cols = (N < 0);
  static constexpr auto dynamic = (dynamic_rows && dynamic_cols);
  static constexpr auto hybrid  = (dynamic_rows || dynamic_cols);
  static constexpr auto immutable = !hybrid;

  template < bool X = immutable, typename = std::enable_if_t<X> >
  constexpr matrix() : data_{} {}

  template < bool X = immutable, typename = std::enable_if_t<X>, typename... Args >
  constexpr explicit matrix(type i, Args&&... args) : data_{{i, std::forward<Args>(args)...}} {
    [this](size_t s) {
      std::fill(data_.d.begin() + s + 1, data_.d.end(), data_.d[s]);
    }(sizeof...(args));
  }

  template < bool X = hybrid, typename = std::enable_if_t<X> >
  constexpr explicit matrix(size_t n)
    : data_{{}, dynamic_rows ? n : static_cast<size_t>(M), dynamic_cols ? n : static_cast<size_t>(N)} 
    { data_.d.resize(data_.m * data_.n); }

  template < bool X = dynamic, typename = std::enable_if_t<X> >
  constexpr explicit matrix(size_t m, size_t n)
    : data_{container_type(m * n), m, n} {}

  template <typename Mat>
  matrix(const matrix_base<Mat>& o) : matrix{o, std::integral_constant<bool, immutable>{}}
  { eval(*this, o, data_); }

  template <typename Mat> matrix(const matrix_base<Mat>& o, std::true_type) {}
  template <typename Mat> matrix(const matrix_base<Mat>& o, std::false_type)
    : data_{container_type(o.rows() * o.cols()), o.rows(), o.cols()} {}

  matrix(const base_type& o) : data_{o.derived().data_} {}

  auto rows() const { return data_.rows_impl(); }
  auto cols() const { return data_.cols_impl(); }

  auto buffer() const { return data_.d.data(); }
  auto buffer()       { return data_.d.data(); }

  auto operator()(size_t row, size_t col) const -> const type&;
  auto operator()(size_t row, size_t col) -> type&;

  auto operator[](size_t n) const -> const type&;
  auto operator[](size_t n) -> type&;

  auto make_eye() -> matrix&;

  template < bool X = immutable, typename = std::enable_if_t<X> >
  static auto eye()         { matrix eye{};  return eye.make_eye(); }

  template < bool X = hybrid,    typename = std::enable_if_t<X> >
  static auto eye(size_t n) { matrix eye{n}; return eye.make_eye(); }

private:
  using container_type = std::conditional_t
    <hybrid,
     std::vector<type>, 
     std::array<type, M * N>
    >;
    
  struct dynamic_data {
    size_t rows_impl() const 
    { return m; } 
    size_t cols_impl() const 
    { return n; } container_type d; size_t m, n; };
  struct static_data {
    size_t rows_impl() const 
    { return M; }
    size_t cols_impl() const 
    { return N; } alignas(IG_VECTORIZE_ALIGN) container_type d; };

  std::conditional_t
    <hybrid, 
     dynamic_data, static_data> data_;
};

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) const -> const type& {
  assert(row < rows() && col < cols() && "Invalid matrix subscript");
  return data_.d[col * rows() + row]; }

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) -> type& {
  assert(row < rows() && col < cols() && "Invalid matrix subscript");
  return data_.d[col * rows() + row]; }

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator[](size_t n) const -> const type& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d[n]; }

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator[](size_t n) -> type& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d[n]; }

// Eye (Identity)
template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::make_eye() -> matrix& {
  size_t i = 0, stride = base_type::diagsize() + 1;
  std::generate(base_type::begin(), base_type::end(), [this, &i, &stride]() { return !(i++ % stride); });
  return *this;
}

} // namespace ig

#endif // IG_MATH_MATRIX_H
