/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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

namespace
{
// Operators
auto operator+(const float8& lhs, const float8& rhs) { return float8{_mm256_add_ps(lhs, rhs)}; }
auto operator-(const float8& lhs, const float8& rhs) { return float8{_mm256_sub_ps(lhs, rhs)}; }
auto operator*(const float8& lhs, const float8& rhs) { return float8{_mm256_mul_ps(lhs, rhs)}; }
auto operator/(const float8& lhs, const float8& rhs) { return float8{_mm256_div_ps(lhs, rhs)}; }

auto operator^(const float8& lhs, const float8& rhs) { return float8{_mm256_xor_ps(lhs, rhs)}; }
auto operator-(const float8& v)                      { return float8{_mm256_xor_ps(v, _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000)))}; }

// Comparison
auto operator==(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_EQ_OQ)}; }
auto operator!=(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_NEQ_OQ)}; }
auto operator< (const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_LT_OQ)}; }
auto operator<=(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_LE_OQ)}; }
auto operator> (const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_GT_OQ)}; }
auto operator>=(const float8& lhs, const float8& rhs) { return bool8{_mm256_cmp_ps(lhs, rhs, _CMP_GE_OQ)}; }

// Relational
auto min(const float8& lhs, const float8& rhs) { return float8{_mm256_min_ps(lhs, rhs)}; }
auto max(const float8& lhs, const float8& rhs) { return float8{_mm256_max_ps(lhs, rhs)}; }

// Arithmetic & Rounding
auto abs(const float8& v)     { return float8{_mm256_and_ps(v, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)))}; }
auto sgnmask(const float8& v) { return float8{_mm256_and_ps(v, _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000)))}; }

auto sqrt(const float8& v)
{ return float8{_mm256_sqrt_ps(v)}; }

// Movement & Shuffling
auto unpacklo(const float8& lhs, const float8& rhs) { return float8{_mm256_unpacklo_ps(lhs, rhs)}; }
auto unpackhi(const float8& lhs, const float8& rhs) { return float8{_mm256_unpackhi_ps(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const float8& v)
{ return float8{_mm256_permute_ps(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const float8& v, const float8& t)
{ return float8{_mm256_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }
//
}

} // namespace ig

#endif // IG_MATH_FLOAT8_H
