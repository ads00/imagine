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


#ifdef _MSC_VER
#include <intrin.h>

static inline int __builtin_ctz(uint32_t x) {
    unsigned long ret;
    _BitScanForward(&ret, x);
    return (int)ret;
}

static inline int __builtin_ctzll(unsigned long long x) {
    unsigned long ret;
    _BitScanForward64(&ret, x);
    return (int)ret;
}

static inline int __builtin_ctzl(unsigned long x) {
    return sizeof(x) == 8 ? __builtin_ctzll(x) : __builtin_ctz((uint32_t)x);
}

static inline int __builtin_clz(uint32_t x) {
    //unsigned long ret;
    //_BitScanReverse(&ret, x);
    //return (int)(31 ^ ret);
    return (int)__lzcnt(x);
}

static inline int __builtin_clzll(unsigned long long x) {
    //unsigned long ret;
    //_BitScanReverse64(&ret, x);
    //return (int)(63 ^ ret);
    return (int)__lzcnt64(x);
}

static inline int __builtin_clzl(unsigned long x) {
    return sizeof(x) == 8 ? __builtin_clzll(x) : __builtin_clz((uint32_t)x);
}

#ifdef __cplusplus
static inline int __builtin_ctzl(unsigned long long x) {
    return __builtin_ctzll(x);
}

static inline int __builtin_clzl(unsigned long long x) {
    return __builtin_clzll(x);
}
#endif
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
