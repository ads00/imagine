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

#ifndef IG_MATH_SSE_B_H
#define IG_MATH_SSE_B_H

namespace ig {

struct packet_bool {
  packet_bool() = default;
  packet_bool(__m128 in) : d{in} {}
  packet_bool(bool x)    : d{x ? _mm_castsi128_ps(_mm_set1_epi32(-1)) : _mm_setzero_ps()} {}

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

// Operators
inline auto operator!(const packet_bool& v) { return packet_bool{_mm_xor_ps(v, packet_bool{std::true_type{}})}; }

inline auto operator&(const packet_bool& lhs, const packet_bool& rhs) { return packet_bool{_mm_and_ps(lhs, rhs)}; }
inline auto operator|(const packet_bool& lhs, const packet_bool& rhs) { return packet_bool{_mm_or_ps(lhs, rhs)}; }
inline auto operator^(const packet_bool& lhs, const packet_bool& rhs) { return packet_bool{_mm_xor_ps(lhs, rhs)}; }

// Comparison
inline auto operator!=(const packet_bool& lhs, const packet_bool& rhs) { return packet_bool{_mm_xor_ps(lhs, rhs)}; }
inline auto operator==(const packet_bool& lhs, const packet_bool& rhs) { return packet_bool{_mm_castsi128_ps(_mm_cmpeq_epi32(lhs, rhs))}; }

// Reduction
inline auto movemask(const packet_bool& v)
{ return _mm_movemask_ps(v); }

inline bool all(const packet_bool& v)  { return movemask(v) == 0xf; }
inline bool any(const packet_bool& v)  { return movemask(v) != 0x0; }
inline bool none(const packet_bool& v) { return movemask(v) == 0x0; }

// Movement & Shifting & Shuffling
inline auto unpacklo(const packet_bool& lhs, const packet_bool& rhs) { return packet_bool{_mm_unpacklo_ps(lhs, rhs)}; }
inline auto unpackhi(const packet_bool& lhs, const packet_bool& rhs) { return packet_bool{_mm_unpackhi_ps(lhs, rhs)}; }

template 
< size_t i0, 
  size_t i1, 
  size_t i2, 
  size_t i3 > 
inline auto shuffle(const packet_bool& v)
{ return packet_bool{_mm_shuffle_epi32(v, _MM_SHUFFLE(i3, i2, i1, i0))}; }

template 
< size_t i0, 
  size_t i1, 
  size_t i2, 
  size_t i3 >
inline auto shuffle(const packet_bool& v, const packet_bool& t)
{ return packet_bool{_mm_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }
inline auto select(const packet_bool& lhs, const packet_bool& rhs, const packet_bool& mask)
{ return packet_bool{_mm_blendv_ps(lhs, rhs, mask)}; }

} // namespace ig

#endif // IG_MATH_SSE_B_H
