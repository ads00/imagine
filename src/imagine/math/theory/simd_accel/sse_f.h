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

#ifndef IG_MATH_SSE_F_H
#define IG_MATH_SSE_F_H

namespace ig {

struct packet_float {
  packet_float() = default;
  packet_float(__m128 in) : d{in} {}
  packet_float(float x)   : d{_mm_set1_ps(x)} {}
  explicit packet_float(float x, float y, float z, float w)
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
inline auto operator+(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_add_ps(lhs, rhs)}; }
inline auto operator-(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_sub_ps(lhs, rhs)}; }
inline auto operator*(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_mul_ps(lhs, rhs)}; }
inline auto operator/(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_div_ps(lhs, rhs)}; }

inline auto operator^(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_xor_ps(lhs, rhs)}; }
inline auto operator-(const packet_float& v)                            { return packet_float{_mm_xor_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)))}; }

// Comparison
inline auto operator==(const packet_float& lhs, const packet_float& rhs) { return packet_bool{_mm_cmpeq_ps(lhs, rhs)}; }
inline auto operator!=(const packet_float& lhs, const packet_float& rhs) { return packet_bool{_mm_cmpneq_ps(lhs, rhs)}; }
inline auto operator< (const packet_float& lhs, const packet_float& rhs) { return packet_bool{_mm_cmplt_ps(lhs, rhs)}; }
inline auto operator>=(const packet_float& lhs, const packet_float& rhs) { return packet_bool{_mm_cmpnlt_ps(lhs, rhs)}; }
inline auto operator> (const packet_float& lhs, const packet_float& rhs) { return packet_bool{_mm_cmpnle_ps(lhs, rhs)}; }
inline auto operator<=(const packet_float& lhs, const packet_float& rhs) { return packet_bool{_mm_cmple_ps(lhs, rhs)}; }

// Relational
inline auto min(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_min_ps(lhs, rhs)}; }
inline auto max(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_max_ps(lhs, rhs)}; }

// Arithmetic & Rounding
inline auto abs(const packet_float& v)     { return packet_float{_mm_and_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)))}; }
inline auto sgnmask(const packet_float& v) { return packet_float{_mm_and_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)))}; }

inline auto sqrt(const packet_float& v)
{ return packet_float{_mm_sqrt_ps(v)}; }
inline auto rcp(const packet_float& v) {
  auto r = _mm_rcp_ps(v); 
  return packet_float{_mm_mul_ps(r, _mm_sub_ps(packet_float{2.f}, _mm_mul_ps(r, v)))}; }
inline auto rsqrt(const packet_float& v) {
  auto r = _mm_rsqrt_ps(v);
  return packet_float{_mm_add_ps(_mm_mul_ps(_mm_set1_ps(1.5f), r),
                     _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(v, _mm_set1_ps(-0.5f)), r), _mm_mul_ps(r, r)))}; }

// Movement & Shifting & Shuffling
inline auto unpacklo(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_unpacklo_ps(lhs, rhs)}; }
inline auto unpackhi(const packet_float& lhs, const packet_float& rhs) { return packet_float{_mm_unpackhi_ps(lhs, rhs)}; }

template 
< size_t i0, 
  size_t i1, 
  size_t i2, 
  size_t i3 > 
inline auto shuffle(const packet_float& v)
{ return packet_float{_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v), _MM_SHUFFLE(i3, i2, i1, i0)))}; }

template 
< size_t i0, 
  size_t i1, 
  size_t i2, 
  size_t i3 >
inline auto shuffle(const packet_float& v, const packet_float& t)
{ return packet_float{_mm_shuffle_ps(v, t, _MM_SHUFFLE(i3, i2, i1, i0))}; }

// Reduction
inline auto vreduce_min(const packet_float& v) { 
  auto h = min(shuffle<1, 0, 3, 2>(v), v); 
  return   min(shuffle<2, 3, 0, 1>(h), h); }
inline auto vreduce_max(const packet_float& v) { 
  auto h = max(shuffle<1, 0, 3, 2>(v), v); 
  return   max(shuffle<2, 3, 0, 1>(h), h); }

inline auto select(const packet_bool& m, const packet_float& t, const packet_float& f) { return packet_float{_mm_or_ps(_mm_and_ps(m, t), _mm_andnot_ps(m, f))}; }
inline auto select_min(const packet_bool& b, const packet_float& v) {
  auto a = select(b, v,  std::numeric_limits<float>::infinity());
  auto m = b & (a == vreduce_min(a));
  return bit_scan_forward(movemask(any(m) ? m : b)); }
inline auto select_max(const packet_bool& b, const packet_float& v) {
  auto a = select(b, v, -std::numeric_limits<float>::infinity());
  auto m = b & (a == vreduce_max(a));
  return bit_scan_forward(movemask(any(m) ? m : b)); }

} // namespace ig

#endif // IG_MATH_SSE_F_H
