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

#ifndef IG_MATH_MATRIX_H
#define IG_MATH_MATRIX_H

#include "imagine/math/linalg/base/alg.h"
#include "imagine/math/linalg/base/alg_block.h"
#include "imagine/math/linalg/base/alg_col.h"
#include "imagine/math/linalg/base/alg_row.h"
#include "imagine/math/linalg/base/alg_diag.h"
#include "imagine/math/linalg/base/alg_trans.h"
#include "imagine/math/linalg/base/alg_relational.h"

#include "imagine/math/linalg/expr/alg_product.h"
#include "imagine/math/linalg/expr/alg_unary.h"
#include "imagine/math/linalg/expr/alg_scalar.h"
#include "imagine/math/linalg/expr/alg_binary.h"

#include <algorithm>
#include <numeric>

namespace ig {

template <typename T_, int M_, int N_>
struct alg_traits< matrix<T_, M_, N_> > {
  using T = T_;
  static constexpr auto M = M_, N = N_;
};

template <typename T, int M = dynamic_sized, int N = M>
class matrix : public alg< matrix<T, M, N> > {
public:
  using base = alg< matrix<T, M, N> >;
  struct order{};

  static constexpr auto dynamic_rows = (M < 0);
  static constexpr auto dynamic_cols = (N < 0);
  static constexpr auto dynamic = (dynamic_rows && dynamic_cols);
  static constexpr auto hybrid  = (dynamic_rows || dynamic_cols);
  static constexpr auto immutable = !hybrid;

  template < typename = std::enable_if_t<immutable> >
  constexpr matrix() : data_{} {}

  template < typename = std::enable_if_t<immutable>, typename... Args >
  constexpr matrix(order, Args&&... args) : data_{} {
    base::initializer::
      auto_construct(*this, std::forward<Args>(args)...);
  }

  template < typename = std::enable_if_t<immutable>, typename... Args >
  constexpr matrix(T i, Args&&... args) 
    : data_{{i, std::forward<Args>(args)...}} {
    [this](size_t s) {
      std::fill(data_.d_.begin() + s + 1, data_.d_.end(), data_.d_[s]);
    }(sizeof...(args));
  }

  template < typename = std::enable_if_t<dynamic> >
  constexpr matrix(size_t m, size_t n)
    : data_{m, n, std::vector<T>(data_.rows_ * data_.cols_)} {}

  template < typename = std::enable_if_t<hybrid> >
  constexpr matrix(size_t n)
    : data_{dynamic_rows ? n : static_cast<size_t>(M), dynamic_cols ? n : static_cast<size_t>(N),
            std::vector<T>(data_.rows_ * data_.cols_)} {}

  template <typename Alg>
  matrix(const alg<Alg>& o) : matrix{o, std::integral_constant<bool, immutable>{}} {
    eval(*this, o, data_);
  }
  template <typename Alg> matrix(const alg<Alg>& o, std::true_type) {}
  template <typename Alg> matrix(const alg<Alg>& o, std::false_type)
    : data_{dynamic_rows ? o.rows() : static_cast<size_t>(M), dynamic_cols ? o.cols() : static_cast<size_t>(N),
            std::vector<T>(data_.rows_ * data_.cols_)} {}

  matrix(const base& o) : data_{o.derived().data_} {}

  constexpr auto rows() const { return data_.rows(); }
  constexpr auto cols() const { return data_.cols(); }

  auto data() const { return data_.d_.data(); }
  auto data()       { return data_.d_.data(); }

  auto operator()(size_t row, size_t col) const -> const T&;
  auto operator()(size_t row, size_t col) -> T&;

  auto operator[](size_t n) const -> const T&;
  auto operator[](size_t n) -> T&;

  auto make_eye() -> matrix&;

  template < typename = std::enable_if_t<immutable> >
  static auto eye()         { matrix eye{}; return eye.make_eye(); }

  template < typename = std::enable_if_t<hybrid> >
  static auto eye(size_t n) { matrix eye{n}; return eye.make_eye(); }

protected:
  struct dynamic_data {
    constexpr auto rows() const { return rows_; } constexpr auto cols() const { return cols_; }
    size_t rows_, cols_; std::vector<T> d_; 
  };
  struct static_data {
    constexpr size_t rows() const { return M; } constexpr size_t cols() const { return N; }
    std::array<T, M * N> d_;
  };
  std::conditional_t<hybrid, dynamic_data, static_data> data_;
};

template <typename T, int M, int N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) const -> const T& {
  assert(row < rows() && col < cols() && "Invalid matrix subscript");
  return data_.d_[col*rows() + row];
}

template <typename T, int M, int N>
auto matrix<T, M, N>::operator()(size_t row, size_t col) -> T& {
  assert(row < rows() && col < cols() && "Invalid matrix subscript");
  return data_.d_[col*rows() + row];
}

template <typename T, int M, int N>
auto matrix<T, M, N>::operator[](size_t n) const -> const T& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d_[n];
}

template <typename T, int M, int N>
auto matrix<T, M, N>::operator[](size_t n) -> T& {
  assert(n < rows() * cols() && "Invalid matrix subscript");
  return data_.d_[n];
}

// Eye (Identity)
template <typename T, int M, int N>
auto matrix<T, M, N>::make_eye() -> matrix& {
  size_t i = 0, stride = diagsize() + 1;
  std::generate(begin(), end(), [&i, &stride, this]() { return !(i++ % stride); });
  return *this;
}

#define IG_ALG_USINGS(Type, TypeSuffix, Size) \
using matrix##Size##TypeSuffix = matrix<Type, Size, Size>; \
using vector##Size##TypeSuffix = matrix<Type, Size, 1>;    \
using row##Size##TypeSuffix    = matrix<Type, 1, Size>;

#define IG_MATRIX_USINGS_SIZES(Type, TypeSuffix) \
using matrix##TypeSuffix = matrix<Type, dynamic_sized, dynamic_sized>; \
using vector##TypeSuffix = matrix<Type, dynamic_sized, 1>;             \
using row##TypeSuffix    = matrix<Type, 1, dynamic_sized>;             \
IG_ALG_USINGS(Type, TypeSuffix, 2) \
IG_ALG_USINGS(Type, TypeSuffix, 3) \
IG_ALG_USINGS(Type, TypeSuffix, 4)

IG_MATRIX_USINGS_SIZES(int, i)
IG_MATRIX_USINGS_SIZES(float, f)
IG_MATRIX_USINGS_SIZES(double, d)
IG_MATRIX_USINGS_SIZES(std::complex<float>, cf)
IG_MATRIX_USINGS_SIZES(std::complex<double>, cd)

#undef IG_MATRIX_USINGS_SIZES
#undef IG_ALG_USINGS

} // namespace ig

#endif // IG_MATH_MATRIX_H
