/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MATRIXPROD_H
#define IG_MATH_MATRIXPROD_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template
< typename Lhs,
  typename Rhs >
struct matrix_traits
<
  matrix_prod<Lhs, Rhs>
>
{
  using value_type = std::common_type_t
    < matrix_t<Lhs>,
      matrix_t<Rhs>
    >;
  static constexpr auto dyn =
    matrix_traits<Lhs>::n_rows == dynamic_size ||
    matrix_traits<Rhs>::n_cols == dynamic_size;
  static constexpr auto n_rows = dyn ? dynamic_size : matrix_traits<Lhs>::n_rows,
                        n_cols = dyn ? dynamic_size : matrix_traits<Rhs>::n_cols;
};

template
< typename l_,
  typename r_ >
class matrix_prod : public matrix_base< matrix_prod<l_, r_> > {
public:
  using matrix_type = concrete_matrix<matrix_prod>;
  explicit matrix_prod(const l_& lhs, const r_& rhs)
    : matrix_prod{
        lhs,
        rhs,
        std::integral_constant<bool, matrix_type::immutable>{}}
  { eval_product(lhs, rhs); }

  matrix_prod(const l_& lhs, const r_& rhs, std::true_type)  : prod_{} {}
  matrix_prod(const l_& lhs, const r_& rhs, std::false_type) : prod_{lhs.rows(), rhs.cols()} {}

  auto rows() const { return prod_.rows(); }
  auto cols() const { return prod_.cols(); }

  decltype(auto) operator()(size_t row, size_t col) const
  { return prod_(row, col); }
  decltype(auto) operator()(size_t row, size_t col)
  { return prod_(row, col); }

  decltype(auto) operator[](size_t n) const
  { return prod_[n]; }
  decltype(auto) operator[](size_t n)
  { return prod_[n]; }

private:
  void eval_product(const l_& lhs, const r_& rhs) {
    for (size_t i = 0; i < lhs.rows(); ++i)
      for (size_t j = 0; j < lhs.cols(); ++j)
        for (size_t k = 0; k < rhs.cols(); ++k)
          prod_[i * rhs.cols() + k] +=
            lhs[i * lhs.cols() + j] *
            rhs[j * rhs.cols() + k];
          /*prod_(i, k) +=
            lhs(i, j) *
            rhs(j, k);*/
  }

  matrix_type prod_;
};

template <typename Lhs, typename Rhs>
constexpr auto operator%(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.cols() == rhs.rows() && "Incoherent matrix-matrix multiplication");
  return matrix_prod
    < Lhs,
      Rhs
    >{lhs.derived(), rhs.derived()};
}

} // namespace ig

#endif // IG_MATH_MATRIXPROD_H
