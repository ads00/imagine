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

#ifndef IG_MATH_RELATIONAL_H
#define IG_MATH_RELATIONAL_H

#include "imagine/math/linalg/base/lin.h"

namespace ig {

template <typename Lhs, typename Rhs>
bool operator<(const lin_base<Lhs>& lhs, const lin_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix comparison");

  auto pair = std::mismatch(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  return (pair.first == lhs.end() || 
         *pair.first < *pair.second);
}

template <typename Lhs, typename Rhs>
auto operator>(const lin_base<Lhs>& lhs, const lin_base<Rhs>& rhs) {
  return rhs < lhs;
}

template <typename Lhs, typename Rhs>
auto operator<=(const lin_base<Lhs>& lhs, const lin_base<Rhs>& rhs) {
  return !(lhs > rhs);
}

template <typename Lhs, typename Rhs>
auto operator>=(const lin_base<Lhs>& lhs, const lin_base<Rhs>& rhs) {
  return !(lhs < rhs);
}

} // namespace ig

#endif // IG_MATH_RELATIONAL_H
