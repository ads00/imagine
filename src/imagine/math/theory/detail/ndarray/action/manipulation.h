/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYMANIPULATION_H
#define IG_MATH_NDARRAYMANIPULATION_H

#include "imagine/math/theory/ndarray.h"

namespace ig {
namespace nd {

template <typename T>
constexpr auto linspace(T start, T end, T count) {
  ndarray<T> arr{std::initializer_list<T>{count}};
  std::generate(
    arr.begin(),
    arr.end(),
    [&, step = (end - start) / (count - 1), i = 0]() mutable { return start + (i++) * step; });
  return arr;
}

// grid

// split
// tile

} // namespace nd
} // namespace ig

#endif // IG_MATH_NDARRAYMANIPULATION_H
