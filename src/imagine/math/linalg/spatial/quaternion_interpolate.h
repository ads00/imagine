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

#ifndef IG_MATH_INTERPOLATE_H
#define IG_MATH_INTERPOLATE_H

namespace ig     {
namespace linalg {

template <typename T>
constexpr auto lerp(const quaternion<T>& lhs, const quaternion<T>& rhs, T t) {
  return normalise(lhs*(T(1) - t) + rhs*t);
}

template <typename T>
auto slerp(const quaternion<T>& lhs, const quaternion<T>& rhs, T t) {
  auto coshalf = dot(lhs, rhs);
  if (std::abs(coshalf) >= T(1)) {
    return lerp(lhs, rhs, t);
  } else {
    auto a = std::acos(coshalf);
    return (lhs*std::sin((T(1) - t) * a) + rhs*std::sin(t * a)) / std::sin(a);
  }
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_INTERPOLATE_H
