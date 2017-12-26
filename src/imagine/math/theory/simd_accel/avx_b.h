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

#ifndef IG_MATH_BOOL8_H
#define IG_MATH_BOOL8_H

namespace ig {

struct bool8 {
  bool8() = default;
  bool8(__m256 in) : d{in} {}
  bool8(bool x)    : d{x ? _mm256_castsi256_ps(_mm256_set1_epi32(-1)) : _mm256_setzero_ps()} {}

  auto& operator[](size_t n) const
  { return d.p[n]; }
  auto& operator[](size_t n)
  { return d.p[n]; }

  operator const __m256&() const { return d.v; }
  operator       __m256&()       { return d.v; }
  operator const __m256i() const { return _mm256_castps_si256(d.v); }

  simd_data
  < __m256,
    int[8],
    32
  > d;
};

// Operators
inline auto operator!(const bool8& v) { return bool8{_mm256_xor_ps(v, bool8{std::true_type{}})}; }

inline auto operator&(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_and_ps(lhs, rhs)}; }
inline auto operator|(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_or_ps(lhs, rhs)}; }
inline auto operator^(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_xor_ps(lhs, rhs)}; }

// Comparison
inline auto operator!=(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_xor_ps(lhs, rhs)}; }
inline auto operator==(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_xor_ps(_mm256_xor_ps(lhs, rhs), bool8{std::true_type{}})}; }

// Reduction
inline auto movemask(const bool8& v)
{ return _mm256_movemask_ps(v); }

inline bool all(const bool8& v)  { return movemask(v) == 0xff; }
inline bool any(const bool8& v)  { return _mm256_testz_ps(v, v) == 0x0; }
inline bool none(const bool8& v) { return _mm256_testz_ps(v, v) != 0x0; }

// Movement & Shifting & Shuffling
inline auto unpacklo(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_unpacklo_ps(lhs, rhs)}; }
inline auto unpackhi(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_unpackhi_ps(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
inline auto shuffle(const bool8& v)
{ return bool8{_mm256_permute_ps(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
inline auto shuffle(const bool8& v, const bool8& t)
{ return bool8{_mm256_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }
inline auto select(const bool8& lhs, const bool8& rhs, const bool8& mask)
{ return bool8{_mm256_blendv_ps(rhs, lhs, mask)}; }

} // namespace ig

#endif // IG_MATH_BOOL8_H
