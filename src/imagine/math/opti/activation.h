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

#ifndef IG_MATH_ACTIVATION_H
#define IG_MATH_ACTIVATION_H

#include "imagine/math/basis.h"

namespace ig {

template <typename Arithmetic> 
constexpr auto logistic_fn(Arithmetic x, Arithmetic L = 1, Arithmetic k = 1) 
{ return L / (1 + std::exp(-k * x)); }
template <typename Arithmetic> 
constexpr auto softplus_fn(Arithmetic x) 
{ return std::log(1 + std::exp(x)); }
template <typename Arithmetic> 
constexpr auto relu_fn(Arithmetic x) 
{ return x < 0 ? x : 0; }
template <typename Arithmetic> 
constexpr auto elu_fn(Arithmetic x, Arithmetic k = 1) 
{ return x < 0 ? k * (std::exp(x) - 1) : x; }

// Radial basis functions
template <typename Arithmetic> 
constexpr auto gaussian_fn(Arithmetic x, Arithmetic k = 1) 
{ return std::exp(-std::pow(k * x, 2)); }
template <typename Arithmetic> 
constexpr auto multiquadratic_fn(Arithmetic x, Arithmetic k = 1) 
{ return std::sqrt(1 + std::pow(k * x, 2)); }

} // namespace ig

#endif // IG_MATH_ACTIVATION_H
