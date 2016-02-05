/*
 Copyright (c) 2015, 2016
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#ifndef COMPUTE_OPERATION_H
#define COMPUTE_OPERATION_H

#include "imagine/compute/linalg/matrix.h"

namespace ig     {
namespace linalg {

template <typename TLhs, typename TRhs>
constexpr auto dot(const alg<TLhs>& lhs, const alg<TRhs>& rhs)
{
  return (lhs % rhs).sum();
}

template <typename TLhs, typename TRhs>
constexpr auto cross(const alg<TLhs>& lhs, const alg<TRhs>& rhs) 
-> vector<std::common_type_t< alg_t<TLhs>, alg_t<TRhs> >, 3>
{
  assert(lhs.vector() && lhs.vecsize() == 3
         && "Cross exists only in three-dimensional space");
  assert(rhs.vector() && rhs.vecsize() == 3
         && "Cross exists only in three-dimensional space");

  return { lhs[1]*rhs[2] - lhs[2]*rhs[1],
           lhs[2]*rhs[0] - lhs[0]*rhs[2],
           lhs[0]*rhs[1] - lhs[1]*rhs[0] };
}

} // namespace linalg
} // namespace ig

#endif // COMPUTE_OPERATION_H
