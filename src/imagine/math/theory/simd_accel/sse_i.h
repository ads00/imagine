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

#ifndef IG_MATH_SSE_I_H
#define IG_MATH_SSE_I_H

namespace ig {

struct packet_int {
  packet_int() = default;
  packet_int(__m128i in) : d{in} {}
  packet_int(__m128 in)  : d{_mm_cvtps_epi32(in)} {}
  packet_int(int x)      : d{_mm_set1_epi32(x)} {}
  explicit packet_int(int x, int y, int z, int w)
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

  simd_data
  < __m128i,
    int[4],
    16
  > d;
};

// Operators
inline auto operator+(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_add_epi32(lhs, rhs)}; }
inline auto operator-(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_sub_epi32(lhs, rhs)}; }
inline auto operator*(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_mullo_epi32(lhs, rhs)}; }

inline auto operator&(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_and_si128(lhs, rhs)}; }
inline auto operator|(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_or_si128(lhs, rhs)}; }
inline auto operator^(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_xor_si128(lhs, rhs)}; }
inline auto operator-(const packet_int& v)                          { return packet_int{_mm_sub_epi32(_mm_setzero_si128(), v)}; }

// Comparison
inline auto operator==(const packet_int& lhs, const packet_int& rhs) { return packet_bool{_mm_castsi128_ps(_mm_cmpeq_epi32(lhs, rhs))}; }
inline auto operator!=(const packet_int& lhs, const packet_int& rhs) { return !(lhs == rhs); }
inline auto operator< (const packet_int& lhs, const packet_int& rhs) { return packet_bool{_mm_castsi128_ps(_mm_cmplt_epi32(lhs, rhs))}; }
inline auto operator>=(const packet_int& lhs, const packet_int& rhs) { return !(lhs <  rhs); }
inline auto operator> (const packet_int& lhs, const packet_int& rhs) { return packet_bool{_mm_castsi128_ps(_mm_cmpgt_epi32(lhs, rhs))}; }
inline auto operator<=(const packet_int& lhs, const packet_int& rhs) { return !(lhs >  rhs); }

// Relational
inline auto min(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_min_epi32(lhs, rhs)}; }
inline auto max(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_max_epi32(lhs, rhs)}; }

// Movement & Shifting & Shuffling
inline auto unpacklo(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_castps_si128(_mm_unpacklo_ps(_mm_castsi128_ps(lhs), _mm_castsi128_ps(rhs)))}; }
inline auto unpackhi(const packet_int& lhs, const packet_int& rhs) { return packet_int{_mm_castps_si128(_mm_unpackhi_ps(_mm_castsi128_ps(lhs), _mm_castsi128_ps(rhs)))}; }

template 
< size_t i0, 
  size_t i1, 
  size_t i2, 
  size_t i3 > 
inline auto shuffle(const packet_int& v)
{ return packet_int{_mm_shuffle_epi32(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template 
< size_t i0, 
  size_t i1, 
  size_t i2, 
  size_t i3 >
inline auto shuffle(const packet_int& v, const packet_int& t)
{ return packet_int{_mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(v), _mm_castsi128_ps(t), _MM_SHUFFLE(i3, i2, i1, i0)))}; }

} // namespace ig

#endif // IG_MATH_SSE_I_H
