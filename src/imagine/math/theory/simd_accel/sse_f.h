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

#ifndef IG_MATH_FLOAT4_H
#define IG_MATH_FLOAT4_H

namespace ig {

struct float4 {
  float4() = default;
  float4(__m128 in) : d{in} {}
  float4(float x)   : d{_mm_set1_ps(x)} {}
  explicit float4(float x, float y, float z, float w)
    : d{_mm_set_ps(
        w,
        z,
        y,
        x)} {}

  auto& operator[](size_t n) const
  { return d.p[n]; }
  auto& operator[](size_t n)
  { return d.p[n]; }

  operator const __m128&() const { return d.v; }
  operator       __m128&()       { return d.v; }

  simd_data
  < __m128,
    float[4],
    16
  > d;
};

// Operators
inline auto operator+(const float4& lhs, const float4& rhs) { return float4{_mm_add_ps(lhs, rhs)}; }
inline auto operator-(const float4& lhs, const float4& rhs) { return float4{_mm_sub_ps(lhs, rhs)}; }
inline auto operator*(const float4& lhs, const float4& rhs) { return float4{_mm_mul_ps(lhs, rhs)}; }
inline auto operator/(const float4& lhs, const float4& rhs) { return float4{_mm_div_ps(lhs, rhs)}; }

inline auto operator^(const float4& lhs, const float4& rhs) { return float4{_mm_xor_ps(lhs, rhs)}; }
inline auto operator-(const float4& v)                      { return float4{_mm_xor_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)))}; }

// Comparison
inline auto operator==(const float4& lhs, const float4& rhs) { return bool4{_mm_cmpeq_ps(lhs, rhs)}; }
inline auto operator!=(const float4& lhs, const float4& rhs) { return bool4{_mm_cmpneq_ps(lhs, rhs)}; }
inline auto operator< (const float4& lhs, const float4& rhs) { return bool4{_mm_cmplt_ps(lhs, rhs)}; }
inline auto operator<=(const float4& lhs, const float4& rhs) { return bool4{_mm_cmple_ps(lhs, rhs)}; }
inline auto operator> (const float4& lhs, const float4& rhs) { return bool4{_mm_cmpnle_ps(lhs, rhs)}; }
inline auto operator>=(const float4& lhs, const float4& rhs) { return bool4{_mm_cmpnlt_ps(lhs, rhs)}; }

// Relational
inline auto min(const float4& lhs, const float4& rhs) { return float4{_mm_min_ps(lhs, rhs)}; }
inline auto max(const float4& lhs, const float4& rhs) { return float4{_mm_max_ps(lhs, rhs)}; }

// Arithmetic & Rounding
inline auto abs(const float4& v)     { return float4{_mm_and_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)))}; }
inline auto sgnmask(const float4& v) { return float4{_mm_and_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)))}; }

inline auto sqrt(const float4& v)
{ return float4{_mm_sqrt_ps(v)}; }

// Movement & Shuffling
inline auto unpacklo(const float4& lhs, const float4& rhs) { return float4{_mm_unpacklo_ps(lhs, rhs)}; }
inline auto unpackhi(const float4& lhs, const float4& rhs) { return float4{_mm_unpackhi_ps(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
inline auto shuffle(const float4& v)
{ return float4{_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v), _MM_SHUFFLE(i3, i2, i1, i0)))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
inline auto shuffle(const float4& v, const float4& t)
{ return float4{_mm_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }

} // namespace ig

#endif // IG_MATH_FLOAT4_H
