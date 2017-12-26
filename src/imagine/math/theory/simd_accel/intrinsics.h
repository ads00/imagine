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
struct simd_data { union alignas(Align) { Simd v; Plain p; }; };

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
