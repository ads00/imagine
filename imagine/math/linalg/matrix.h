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

#include "imagine/math/linalg/base/lin.h"
#include "imagine/math/linalg/base/lin_block.h"
#include "imagine/math/linalg/base/lin_col.h"
#include "imagine/math/linalg/base/lin_row.h"
#include "imagine/math/linalg/base/lin_diag.h"
#include "imagine/math/linalg/base/lin_trans.h"
#include "imagine/math/linalg/base/lin_relational.h"

#include "imagine/math/linalg/expr/lin_product.h"
#include "imagine/math/linalg/expr/lin_unary.h"
#include "imagine/math/linalg/expr/lin_binary.h"

namespace ig {

template <typename T_, int32_t M_, int32_t N_>
struct lin_traits< matrix<T_, M_, N_> > {
  using T = T_;
  static constexpr auto M = M_, N = N_;
};

template <typename T, int32_t M = dynamic_sized, int32_t N = M>
class matrix : public lin_base< matrix<T, M, N> > {
public:
  using base_type 
    = lin_base< matrix<T, M, N> >;

  static constexpr auto dynamic_rows = (M < 0);
  static constexpr auto dynamic_cols = (N < 0);
  static constexpr auto dynamic = (dynamic_rows && dynamic_cols);
  static constexpr auto hybrid  = (dynamic_rows || dynamic_cols);
  static constexpr auto immutable = !hybrid;

  template < bool X = immutable, typename = std::enable_if_t<X> >
  constexpr matrix() : data_{} {}

  template < bool X = immutable, typename = std::enable_if_t<X>, typename... Args >
  constexpr explicit matrix(T i, Args&&... args) : data_{{i, std::forward<Args>(args)...}} {
    [this](size_t s) {
      std::fill(data_.d.begin() + s + 1, data_.d.end(), data_.d[s]);
    }(sizeof...(args));
  }

  template < bool X = hybrid, typename = std::enable_if_t<X> >
  constexpr explicit matrix(size_t n)
    : data_{dynamic_rows ? n : static_cast<size_t>(M), dynamic_cols ? n : static_cast<size_t>(N), {}} 
    { data_.d.resize(data_.m * data_.n); }

  template < bool X = dynamic, typename = std::enable_if_t<X> >
  constexpr explicit matrix(size_t m, size_t n)
    : data_{m, n, std::vector<T>(m * n)} {}

  template <typename Lin>
  matrix(const lin_base<Lin>& o) : matrix{o, std::integral_constant<bool, immutable>{}}
  { eval(*this, o, data_); }

  template <typename Lin> matrix(const lin_base<Lin>& o, std::true_type) {}
  template <typename Lin> matrix(const lin_base<Lin>& o, std::false_type)
    : data_{o.rows(), o.cols(), std::vector<T>(o.rows() * o.cols())} {}

  matrix(const base_type& o) : data_{o.derived().data_} {}

  auto rows() const { return data_.rows_impl(); }
  auto cols() const { return data_.cols_impl(); }

  auto buffer() const { return data_.d.data(); }
  auto buffer()       { return data_.d.data(); }

  auto operator()(size_t row, size_t col) const -> const T&;
  auto operator()(size_t row, size_t col) -> T&;

  auto operator[](size_t n) const -> const T&;
  auto operator[](size_t n) -> T&;

  auto make_eye() -> matrix&;

  template < bool X = immutable, typename = std::enable_if_t<X> >
  static auto eye()         { matrix eye{}; return eye.make_eye(); }

  template < bool X = hybrid,    typename = std::enable_if_t<X> >
  static auto eye(size_t n) { matrix eye{n}; return eye.make_eye(); }

private:
  struct dynamic_data {
    auto rows_impl() const { return m; } 
    auto cols_impl() const { return n; }
      size_t m, n;  std::vector<T> d; };
  struct static_data {
    auto rows_impl() const { return static_cast<size_t>(M); }
    auto cols_impl() const { return static_cast<size_t>(N); }
      std::array<T, M * N> d; };
  std::conditional_t<hybrid, dynamic_data, static_data> data_;
};

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) const -> const T& {
  assert(row < rows() && col < cols() && "Invalid matrix subscript");
  return data_.d[col * rows() + row];
}

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) -> T& {
  assert(row < rows() && col < cols() && "Invalid matrix subscript");
  return data_.d[col * rows() + row];
}

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator[](size_t n) const -> const T& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d[n];
}

template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::operator[](size_t n) -> T& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d[n];
}

// Eye (Identity)
template <typename T, int32_t M, int32_t N>
auto matrix<T, M, N>::make_eye() -> matrix& {
  size_t i = 0, stride = base_type::diagsize() + 1;
  std::generate(base_type::begin(), base_type::end(), [this, &i, &stride]() { return !(i++ % stride); });
  return *this;
}

#define IG_LIN_USINGS(Type, TypeSuffix, Size) \
using matrix##Size##TypeSuffix = matrix<Type, Size, Size>; \
using colvec##Size##TypeSuffix = matrix<Type, Size, 1>;    \
using rowvec##Size##TypeSuffix = matrix<Type, 1, Size>;

#define IG_MATRIX_USINGS_SIZES(Type, TypeSuffix) \
using matrix##TypeSuffix = matrix<Type, dynamic_sized, dynamic_sized>; \
using colvec##TypeSuffix = matrix<Type, dynamic_sized, 1>;             \
using rowvec##TypeSuffix = matrix<Type, 1, dynamic_sized>;             \
IG_LIN_USINGS(Type, TypeSuffix, 2) \
IG_LIN_USINGS(Type, TypeSuffix, 3) \
IG_LIN_USINGS(Type, TypeSuffix, 4)

IG_MATRIX_USINGS_SIZES(int, i)
IG_MATRIX_USINGS_SIZES(float, f)
IG_MATRIX_USINGS_SIZES(double, d)
IG_MATRIX_USINGS_SIZES(std::complex<float>, cf)
IG_MATRIX_USINGS_SIZES(std::complex<double>, cd)

#undef IG_MATRIX_USINGS_SIZES
#undef IG_LIN_USINGS

} // namespace ig

#endif // IG_MATH_MATRIX_H