/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_ANALYSIS_H
#define IG_MATH_ANALYSIS_H

#include "imagine/math/linalg/op.h"
#include "imagine/math/linalg/det.h"
#include "imagine/math/linalg/inv.h"

namespace ig  {
namespace lin {

template <typename Mat>
constexpr auto norm(const matrix_base<Mat>& mat)
{ return std::sqrt(dot(mat, mat)); }

template <typename Mat>
constexpr auto normalise(const matrix_base<Mat>& mat)
{ return mat / norm(mat); }

template <typename Mat>
constexpr auto trace(const matrix_base<Mat>& mat)
{ return mat.diag().sum(); }

// null space

} // namespace lin
} // namespace ig

#endif // IG_MATH_ANALYSIS_H
