/*
 Copyright (c) 2018
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

#ifndef IG_MATH_INT4_H
#define IG_MATH_INT4_H

namespace ig {

struct int4 {
  int4() = default;
  int4(__m128i in) : d{in} {}
  int4(__m128  in) : d{_mm_cvtps_epi32(in)} {}
  int4(int x)      : d{_mm_set1_epi32(x)} {}
  explicit int4(int x, int y, int z, int w)
    : d{_mm_set_epi32(
        w,
        z,
        y,
        x)} {}

  auto& operator[](size_t n) const
  { return d.p[n]; }
  auto& operator[](size_t n)
  { return d.p[n]; }

  operator const __m128i&() const { return d.v; }
  operator       __m128i&()       { return d.v; }
  operator const __m128  () const { return _mm_castsi128_ps(d.v); }

  simd_data
  < __m128i,
    int[4],
    16
  > d;
};

namespace
{
// Operators
auto operator+(const int4& lhs, const int4& rhs) { return int4{_mm_add_epi32(lhs, rhs)}; }
auto operator-(const int4& lhs, const int4& rhs) { return int4{_mm_sub_epi32(lhs, rhs)}; }
auto operator*(const int4& lhs, const int4& rhs) {
  auto l = _mm_mul_epu32(lhs, rhs);
  auto h = _mm_mul_epu32(_mm_srli_si128(lhs, 4), _mm_srli_si128(rhs, 4));
  return int4{_mm_unpacklo_epi32(_mm_shuffle_epi32(l, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_epi32(h, _MM_SHUFFLE(0, 0, 2, 0)))}; }

auto operator&(const int4& lhs, const int4& rhs) { return int4{_mm_and_si128(lhs, rhs)}; }
auto operator|(const int4& lhs, const int4& rhs) { return int4{_mm_or_si128(lhs, rhs)}; }
auto operator^(const int4& lhs, const int4& rhs) { return int4{_mm_xor_si128(lhs, rhs)}; }
auto operator-(const int4& v)                    { return int4{_mm_sub_epi32(_mm_setzero_si128(), v)}; }

// Comparison
auto operator==(const int4& lhs, const int4& rhs) { return bool4{_mm_castsi128_ps(_mm_cmpeq_epi32(lhs, rhs))}; }
auto operator!=(const int4& lhs, const int4& rhs) { return !(lhs == rhs); }
auto operator< (const int4& lhs, const int4& rhs) { return bool4{_mm_castsi128_ps(_mm_cmplt_epi32(lhs, rhs))}; }
auto operator>=(const int4& lhs, const int4& rhs) { return !(lhs < rhs); }
auto operator> (const int4& lhs, const int4& rhs) { return bool4{_mm_castsi128_ps(_mm_cmpgt_epi32(lhs, rhs))}; }
auto operator<=(const int4& lhs, const int4& rhs) { return !(lhs > rhs); }

// Relational
auto min(const int4& lhs, const int4& rhs) { return select(lhs < rhs, lhs, rhs); }
auto max(const int4& lhs, const int4& rhs) { return select(lhs > rhs, lhs, rhs); }

// Movement & Shuffling
auto unpacklo(const int4& lhs, const int4& rhs) { return int4{_mm_unpacklo_epi32(lhs, rhs)}; }
auto unpackhi(const int4& lhs, const int4& rhs) { return int4{_mm_unpackhi_epi32(lhs, rhs)}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const int4& v)
{ return int4{_mm_shuffle_epi32(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template
< size_t i0,
  size_t i1,
  size_t i2,
  size_t i3 >
auto shuffle(const int4& v, const int4& t)
{ return int4{_mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(v), _mm_castsi128_ps(t), _MM_SHUFFLE(i3, i2, i1, i0)))}; }
//
}

} // namespace ig

#endif // IG_MATH_INT4_H
