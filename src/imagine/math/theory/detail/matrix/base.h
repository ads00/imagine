/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MATRIXBASE_H
#define IG_MATH_MATRIXBASE_H

#include "imagine/math/basis.h"
#include "imagine/math/theory/detail/xpr.h"

#include <vector>
#include <array>

namespace ig {

constexpr size_t dynamic_size = 0;
template
< typename T,
  size_t M,
  size_t N >
class matrix;
template <typename T, size_t N = dynamic_size> using colvec = matrix<T, N, 1>;
template <typename T, size_t N = dynamic_size> using rowvec = matrix<T, 1, N>;

template
< typename Mat,
  typename FunctionObject >
class matrix_unary;
template
< typename Lhs,
  typename Rhs,
  typename FunctionObject >
class matrix_binary;
template
< typename Lhs,
  typename Rhs >
class matrix_prod;
template <typename Mat> class matrix_trans;

template <typename Xpr> class matrix_block;
template <typename Xpr> class matrix_col;
template <typename Xpr> class matrix_row;
template <typename Xpr> class matrix_diag;

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

template <typename D>
class matrix_base : public xpr<D> {
public:
  using value_type = matrix_t<D>;

  using base = xpr<D>;
  using base::derived;
  template <typename Xpr> using iterator = typename base::template iterator<Xpr>;

  auto begin() const { return iterator<const D>{derived(), 0}; }
  auto begin()       { return iterator<D>{derived(), 0}; }
  auto end() const   { return iterator<const D>{derived(), size()}; }
  auto end()         { return iterator<D>{derived(), size()}; }

  auto rows() const { return derived().rows(); }
  auto cols() const { return derived().cols(); }
  auto size() const { return rows() * cols(); }

  auto diag_size() const { return std::min(rows(), cols()); }
  auto vec_size() const  { return std::max(rows(), cols()); }

  auto square() const { return rows() == cols(); }
  auto vector() const { return rows() == 1 || cols() == 1; }

  auto col(size_t n) const { return matrix_col<const D>{derived(), n}; }
  auto col(size_t n)       { return matrix_col<D>      {derived(), n}; }
  auto row(size_t n) const { return matrix_row<const D>{derived(), n}; }
  auto row(size_t n)       { return matrix_row<D>      {derived(), n}; }

  auto diag() const { return matrix_diag<const D> {derived()}; }
  auto diag()       { return matrix_diag<D>       {derived()}; }
  auto t() const    { return matrix_trans<const D>{derived()}; }
  auto t()          { return matrix_trans<D>      {derived()}; }

  auto head(size_t n, size_t m) const { return matrix_block<const D>{derived(), 0, 0, std::min(n, rows()), std::min(m, cols())}; }
  auto head(size_t n, size_t m)       { return matrix_block<D>      {derived(), 0, 0, std::min(n, rows()), std::min(m, cols())}; }
  auto tail(size_t n, size_t m) const { return matrix_block<const D>{derived(), std::max(0, rows() - n), std::max(0, cols() - m), std::max(1, rows() - n), std::max(1, cols() - m)}; }
  auto tail(size_t n, size_t m)       { return matrix_block<D>      {derived(), std::max(0, rows() - n), std::max(0, cols() - m), std::max(1, rows() - n), std::max(1, cols() - m)}; }

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
  auto& operator*=(const matrix_base<Mat>& mat) { return derived() = std::move(*this) * mat; }
  template <typename Mat>
  auto& operator/=(const matrix_base<Mat>& mat) { return derived() = std::move(*this) / mat; }
  template <typename Mat>
  auto& operator%=(const matrix_base<Mat>& mat) { return derived() = std::move(*this) % mat; }

  auto sum() const  -> value_type;
  auto prod() const -> value_type;
  auto mean() const -> value_type;

  class initializer {
  public:
    explicit initializer(D& mat)
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
    D& mat_;
    size_t row_, col_, curr_;
  };

  auto operator<<(value_type val) { return initializer{derived()}, val; }
};

template <typename Gen, typename Mat>
void eval_helper(matrix_base<Gen>& ev, const matrix_base<Mat>& mat) {
  assert(
    ev.rows() == mat.rows() &&
    ev.cols() == mat.cols()
    && "Incoherent algebraic evaluation");

  for (size_t i = 0; i < ev.size(); ++i)
    ev [i] =
    mat[i];
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
template <typename D>
auto matrix_base<D>::sum() const -> value_type {
  return std::accumulate(
    begin(),
    end(),
    value_type(0));
}

template <typename D>
auto matrix_base<D>::prod() const -> value_type {
  return std::accumulate(
    begin(),
    end(),
    value_type(1),
    std::multiplies<>{});
}

template <typename D>
auto matrix_base<D>::mean() const -> value_type
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
