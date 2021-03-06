/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_BOOL4_H
#define IG_MATH_BOOL4_H

namespace ig {

struct bool4 {
  bool4() = default;
  bool4(__m128 in) : d{in} {}
  bool4(bool x)    : d{x ? _mm_castsi128_ps(_mm_set1_epi32(-1)) : _mm_setzero_ps()} {}

  auto& operator[](size_t n) const
  { return d.p[n]; }
  auto& operator[](size_t n)
  { return d.p[n]; }

  operator const __m128&() const { return d.v; }
  operator       __m128&()       { return d.v; }
  operator const __m128i() const { return _mm_castps_si128(d.v); }

  simd_data
  < __m128,
    int[4],
    16
  > d;
};

namespace
{
// Selection
auto select(const __m128& lhs, const __m128& rhs, const __m128& mask)
{
#if defined \
    (__SSE4_1__)
  return _mm_blendv_ps(
    lhs,
    rhs,
    mask);
#else
  return _mm_or_ps(
    _mm_and_ps   (mask, rhs),
    _mm_andnot_ps(mask, lhs));
#endif
}

// Operators
auto operator!(const bool4& v) { return bool4{_mm_xor_ps(v, bool4{std::true_type{}})}; }

auto operator&(const bool4& lhs, const bool4& rhs) { return bool4{_mm_and_ps(lhs, rhs)}; }
auto operator|(const bool4& lhs, const bool4& rhs) { return bool4{_mm_or_ps(lhs, rhs)}; }
auto operator^(const bool4& lhs, const bool4& rhs) { return bool4{_mm_xor_ps(lhs, rhs)}; }

// Comparison
auto operator!=(const bool4& lhs, const bool4& rhs) { return bool4{_mm_xor_ps(lhs, rhs)}; }
auto operator==(const bool4& lhs, const bool4& rhs) { return bool4{_mm_castsi128_ps(_mm_cmpeq_epi32(lhs, rhs))}; }

// Reduction
auto movemask(const bool4& v)
{ return _mm_movemask_ps(v); }

bool all(const bool4& v)  { return movemask(v) == 0xf; }
bool any(const bool4& v)  { return movemask(v) != 0x0; }
bool none(const bool4& v) { return movemask(v) == 0x0; }

// Movement & Shuffling
auto unpacklo(const bool4& lhs, const bool4& rhs) { return bool4{_mm_unpacklo_ps(lhs, rhs)}; }
auto unpackhi(const bool4& lhs, const bool4& rhs) { return bool4{_mm_unpackhi_ps(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const bool4& v)
{ return bool4{_mm_shuffle_epi32(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const bool4& v, const bool4& t)
{ return bool4{_mm_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }
//
}

} // namespace ig

#endif // IG_MATH_BOOL4_H
