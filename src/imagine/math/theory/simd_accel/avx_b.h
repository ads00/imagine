/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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

namespace
{
// Selection
auto select(const __m256& lhs, const __m256& rhs, const __m256& mask)
{
  return _mm256_blendv_ps(rhs, lhs, mask);
}

// Operators
auto operator!(const bool8& v) { return bool8{_mm256_xor_ps(v, bool8{std::true_type{}})}; }
auto operator~(const bool8& v) { return bool8{_mm256_xor_ps(v, bool8{std::false_type{}})}; }

auto operator&(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_and_ps(lhs, rhs)}; }
auto operator|(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_or_ps(lhs, rhs)}; }
auto operator^(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_xor_ps(lhs, rhs)}; }

// Comparison
auto operator!=(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_xor_ps(lhs, rhs)}; }
auto operator==(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_xor_ps(_mm256_xor_ps(lhs, rhs), bool8{std::true_type{}})}; }

// Reduction
auto movemask(const bool8& v)
{ return _mm256_movemask_ps(v); }

bool all(const bool8& v)  { return _mm256_testc_ps(v, bool8{std::true_type{}}); }
bool any(const bool8& v)  { return _mm256_testz_ps(v, v) == 0x0; }
bool none(const bool8& v) { return _mm256_testz_ps(v, v) != 0x0; }

// Movement & Shuffling
auto unpacklo(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_unpacklo_ps(lhs, rhs)}; }
auto unpackhi(const bool8& lhs, const bool8& rhs) { return bool8{_mm256_unpackhi_ps(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const bool8& v)
{ return bool8{_mm256_permute_ps(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const bool8& v, const bool8& t)
{ return bool8{_mm256_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }
//
}

} // namespace ig

#endif // IG_MATH_BOOL8_H
