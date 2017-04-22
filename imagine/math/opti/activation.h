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

#include "imagine/ig.h"

namespace ig {

template <typename T> constexpr auto logistic_fn(T x, T L = 1, T k = 1) { return L / (1 + std::exp(-k * x)); }
template <typename T> constexpr auto softplus_fn(T x)                   { return std::log(1 + std::exp(x)); }
template <typename T> constexpr auto relu_fn(T x)                       { return std::max(x, T(0)); }
template <typename T> constexpr auto elu_fn(T x, T k = 1)               { return x < 0 ? k * (std::exp(x) - 1) : x; }

// Radial basis functions
template <typename T> constexpr auto gaussian_fn(T x, T k = 1)       { return std::exp(-std::pow(k * x, 2)); }
template <typename T> constexpr auto multiquadratic_fn(T x, T k = 1) { return std::sqrt(1 + std::pow(k * x, 2)); }
template <typename T> constexpr auto polyharmonic_fn(T x, T k = 1)   { return r < 1 ? std::pow(x, k - 1) * std::log(std::pow(x, x)) : std::pow(x, k) * std::log(x); }

} // namespace ig

#endif // IG_MATH_ACTIVATION_H
