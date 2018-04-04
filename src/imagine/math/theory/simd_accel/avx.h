/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_AVX_H
#define IG_MATH_AVX_H

#include "imagine/math/theory/simd_accel/intrinsics.h"

#include "imagine/math/theory/simd_accel/sse_b.h"
#include "imagine/math/theory/simd_accel/sse_f.h"
#include "imagine/math/theory/simd_accel/sse_i.h"

#include "imagine/math/theory/simd_accel/avx_b.h"
#include "imagine/math/theory/simd_accel/avx_f.h"
#if defined(__AVX2__)
#include "imagine/math/theory/simd_accel/avx_i.h"
#endif

//
// AVX packet
using packet = ig::float8;

#endif // IG_MATH_AVX_H
