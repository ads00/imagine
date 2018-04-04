/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_INT8_H
#define IG_MATH_INT8_H

namespace ig {

struct int8 {
  int8() = default;
  int8(__m256i in) : d{in} {}
  int8(__m256  in) : d{_mm256_cvtps_epi32(in)} {}
  int8(int x)      : d{_mm256_set1_epi32(x)} {}
  explicit int8(int a, int b, int c, int d, int e, int f, int g, int h)
    : d{_mm256_setr_epi32(
        a, b,
        c, d,
        e, f,
        g, h)} {}

  auto& operator[](size_t n) const
  { return d.p[n]; }
  auto& operator[](size_t n)
  { return d.p[n]; }

  operator const __m256i&() const { return d.v; }
  operator       __m256i&()       { return d.v; }
  operator const __m256  () const { return _mm256_castsi256_ps(d.v); }

  simd_data
  < __m256i,
    int[8],
    32
  > d;
};

namespace
{
// Operators
auto operator+(const int8& lhs, const int8& rhs) { return int8{_mm256_add_epi32(lhs, rhs)}; }
auto operator-(const int8& lhs, const int8& rhs) { return int8{_mm256_sub_epi32(lhs, rhs)}; }
auto operator*(const int8& lhs, const int8& rhs) { return int8{_mm256_mullo_epi32(lhs, rhs)}; }

auto operator&(const int8& lhs, const int8& rhs) { return int8{_mm256_and_si256(lhs, rhs)}; }
auto operator|(const int8& lhs, const int8& rhs) { return int8{_mm256_or_si256(lhs, rhs)}; }
auto operator^(const int8& lhs, const int8& rhs) { return int8{_mm256_xor_si256(lhs, rhs)}; }
auto operator-(const int8& v)                    { return int8{_mm256_sub_epi32(_mm256_setzero_si256(), v)}; }

// Comparison
auto operator==(const int8& lhs, const int8& rhs) { return bool8{_mm256_castsi256_ps(_mm256_cmpeq_epi32(lhs, rhs))}; }
auto operator!=(const int8& lhs, const int8& rhs) { return !(lhs == rhs); }
auto operator> (const int8& lhs, const int8& rhs) { return bool8{_mm256_castsi256_ps(_mm256_cmpgt_epi32(lhs, rhs))}; }
auto operator<=(const int8& lhs, const int8& rhs) { return !(lhs > rhs); }
auto operator< (const int8& lhs, const int8& rhs) { return rhs > lhs; }
auto operator>=(const int8& lhs, const int8& rhs) { return !(lhs < rhs); }

// Relational
auto min(const int8& lhs, const int8& rhs) { return int8{_mm256_min_epi32(lhs, rhs)}; }
auto max(const int8& lhs, const int8& rhs) { return int8{_mm256_max_epi32(lhs, rhs)}; }

// Movement & Shuffling
auto unpacklo(const int8& lhs, const int8& rhs) { return int8{_mm256_unpacklo_epi32(lhs, rhs)}; }
auto unpackhi(const int8& lhs, const int8& rhs) { return int8{_mm256_unpackhi_epi32(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const int8& v)
{ return int8{_mm256_shuffle_epi32(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const int8& v, const int8& t)
{ return int8{_mm256_castps_si256(_mm256_shuffle_ps(_mm256_castsi256_ps(v), _mm256_castsi256_ps(t), _MM_SHUFFLE(i3, i2, i1, i0)))}; }
//
}

} // namespace ig

#endif // IG_MATH_INT8_H
