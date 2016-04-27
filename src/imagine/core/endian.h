/*
 Copyright (c) 2015, 2016
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

#ifndef IG_CORE_ENDIAN_H
#define IG_CORE_ENDIAN_H

#include "imagine.h"

namespace ig     {
namespace endian {

void helper(const uint8_t* src, uint8_t* dst, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    dst[i] = src[size - 1 - i];
  }
}

template <typename T> constexpr void swap(T src, uint8_t* dst) {
  helper(reinterpret_cast<const uint8_t*>(&src), dst, sizeof(T));
}

template <typename T> constexpr auto swap(T src) {}

#if IG_BIG_ENDIAN
template <typename T> constexpr auto big(T src)    { return src; };
template <typename T> constexpr auto little(T src) { return swap<T>(src); }
#else
template <typename T> constexpr auto big(T src)    { return swap<T>(src) };
template <typename T> constexpr auto little(T src) { return src; }
#endif

template <> constexpr auto swap<uint64_t>(uint64_t src) {
  return ((src & 0x00000000000000ff) << 56) |
         ((src & 0x000000000000ff00) << 40) |
         ((src & 0x0000000000ff0000) << 24) |
         ((src & 0x00000000ff000000) << 8)  |
         ((src & 0x000000ff00000000) >> 8)  |
         ((src & 0x0000ff0000000000) >> 24) |
         ((src & 0x00ff000000000000) >> 40) |
         ((src & 0xff00000000000000) >> 56);
}

template <> constexpr auto swap<uint32_t>(uint32_t src) {
  return ((src & 0x000000ff) << 24) |
         ((src & 0x0000ff00) << 8)  |
         ((src & 0x00ff0000) >> 8)  |
         ((src & 0xff000000) >> 24);
}

template <> constexpr auto swap<uint16_t>(uint16_t src) {
  return ((src & 0x00ff) << 8) |
         ((src & 0xff00) >> 8);
}

template <> constexpr auto swap<int64_t>(int64_t src) { return swap<uint64_t>(src); }
template <> constexpr auto swap<int32_t>(int32_t src) { return swap<uint32_t>(src); }
template <> constexpr auto swap<int16_t>(int16_t src) { return swap<uint16_t>(src); }

} // namespace endian
} // namespace ig

#endif // IG_CORE_ENDIAN_H
