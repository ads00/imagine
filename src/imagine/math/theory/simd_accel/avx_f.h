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

#ifndef IG_MATH_FLOAT8_H
#define IG_MATH_FLOAT8_H

namespace ig {

struct float8 {
  float8() = default;
  float8(__m256 in) : d{in} {}
  float8(float x)   : d{_mm256_set1_ps(x)} {}
  explicit float8(float a, float b, float c, float d, float e, float f, float g, float h)
    : d{_mm256_setr_ps(
        a, b,
        c, d,
        e, f,
        g, h)} {}

  auto operator[](size_t n) const
  { return d.p[n]; }
  auto& operator[](size_t n)
  { return d.p[n]; }

  operator const __m256&() const { return d.v; }
  operator       __m256&()       { return d.v; }

  simd_data
  < __m256,
    float[8],
    32
  > d;
};

// Operators
inline auto operator+(const float8& lhs, const float8& rhs) { return float8{_mm256_add_ps(lhs, rhs)}; }
inline auto operator-(const float8& lhs, const float8& rhs) { return float8{_mm256_sub_ps(lhs, rhs)}; }
inline auto operator*(const float8& lhs, const float8& rhs) { return float8{_mm256_mul_ps(lhs, rhs)}; }
inline auto operator/(const float8& lhs, const float8& rhs) { return float8{_mm256_div_ps(lhs, rhs)}; }

inline auto operator^(const float8& lhs, const float8& rhs) { return float8{_mm256_xor_ps(lhs, rhs)}; }
inline auto operator-(const float8& v)                      { return float8{_mm256_xor_ps(v, _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000)))}; }

// Comparison
inline auto operator==(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_EQ_OQ)}; }
inline auto operator!=(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_NEQ_OQ)}; }
inline auto operator< (const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_LT_OQ)}; }
inline auto operator<=(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_LE_OQ)}; }
inline auto operator> (const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_GT_OQ)}; }
inline auto operator>=(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_GE_OQ)}; }

// Relational
inline auto min(const float8& lhs, const float8& rhs) { return float8{_mm256_min_ps(lhs, rhs)}; }
inline auto max(const float8& lhs, const float8& rhs) { return float8{_mm256_max_ps(lhs, rhs)}; }

// Arithmetic & Rounding
inline auto abs(const float8& v)     { return float8{_mm256_and_ps(v, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)))}; }
inline auto sgnmask(const float8& v) { return float8{_mm256_and_ps(v, _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000)))}; }

inline auto sqrt(const float8& v)
{ return float8{_mm256_sqrt_ps(v)}; }

// Movement & Shuffling
inline auto unpacklo(const float8& lhs, const float8& rhs) { return float8{_mm256_unpacklo_ps(lhs, rhs)}; }
inline auto unpackhi(const float8& lhs, const float8& rhs) { return float8{_mm256_unpackhi_ps(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
inline auto shuffle(const float8& v)
{ return float8{_mm256_permute_ps(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
inline auto shuffle(const float8& v, const float8& t)
{ return float8{_mm256_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }

} // namespace ig

#endif // IG_MATH_FLOAT8_H
