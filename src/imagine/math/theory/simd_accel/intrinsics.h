/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_INTRINSICS_H
#define IG_MATH_INTRINSICS_H

#include "imagine/ig.h"
#include <immintrin.h>

#if defined(__AVX__) || defined(__AVX2__)
# define IG_SSE
# define IG_AVX
# define IG_VECTOR_SIMD 32
# define IG_PACKET_WIDE  8
#elif defined(__SSE2__) || defined(__SSE3__) || defined(__SSSE3__) || defined(__SSE4_1__) || defined(__SSE4_2__) || defined(IG_X86_64)
# define IG_SSE
# define IG_VECTOR_SIMD 16
# define IG_PACKET_WIDE  4
#else
# define IG_VECTOR_SIMD  0
# define IG_PACKET_WIDE  1
#endif

namespace ig {

inline auto clz(size_t v) { return __builtin_clz(v); }
inline auto ctz(size_t v) { return __builtin_ctz(v); }
inline auto ctz_f(size_t& v) {
  auto i = ctz(v);
  v &= v - 1;
  return i;
}

template
< typename Simd,
  typename Plain,
  uint32_t Align >
struct simd_data { union { Simd v; alignas(Align) Plain p; }; };

} // namespace ig

#if defined(IG_X86)

inline void prefetch_l1(const void* ptr)  { _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_T0); }
inline void prefetch_l2(const void* ptr)  { _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_T1); }
inline void prefetch_l3(const void* ptr)  { _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_T2); }
inline void prefetch_ex(const void* ptr)  { _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_T0); }
inline void prefetch_nta(const void* ptr) { _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_NTA); }

# if defined  (IG_AVX)
#  include "imagine/math/theory/simd_accel/avx.h"
# elif defined(IG_SSE)
#  include "imagine/math/theory/simd_accel/sse.h"
# endif
#else

#endif

#endif // IG_MATH_INTRINSICS_H
