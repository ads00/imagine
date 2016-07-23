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

#ifndef IG_MATH_KERNEL_H
#define IG_MATH_KERNEL_H

#include "imagine/ig.h"

#include <cmath>
#include <limits>
#include <complex>

namespace ig {

template <typename T> constexpr T pi          = T(3.141592653589793);
template <typename T> constexpr T two_pi      = T(6.283185307179586);
template <typename T> constexpr T half_pi     = T(1.570796326794897);
template <typename T> constexpr T sqrt_two_pi = T(2.506628274631000);
template <typename T> constexpr T tau         = T(6.283185307179586);
template <typename T> constexpr T elog        = T(2.718281828459045);
template <typename T> constexpr T sqrt2       = T(1.414213562373095);
template <typename T> constexpr T golden      = T(1.618033988749894);
template <typename T> constexpr T omega       = T(0.567143290409783);

// Mathematical constants
template <typename T> constexpr T euler   = T(2.718281828459045);
template <typename T> constexpr T apery   = T(1.202056903159594);
template <typename T> constexpr T conway  = T(1.303577269034296);

// Physical constants
template <typename T> constexpr T light_speed = T(299792458);       // speed of light in vaccum in m.s^-1
template <typename T> constexpr T atomic_mass = T(1.660538921e-27); // atomic mass constant in kg
template <typename T> constexpr T gravity     = T(6.67384e-11);     // Newtonian constant of gravitation in m^3.kg^-1.s^-2
template <typename T> constexpr T planck      = T(6.62606957e-34);  // Planck constant in J.s
template <typename T> constexpr T boltzmann   = T(1.3806488e-23);   // Boltzmann constant in J.K-1
template <typename T> constexpr T avogadro    = T(6.02214129e23);   // Avogadro's number in mol-1

template <typename T>
constexpr int32_t sign(T val) { return (T(0) < val) - (val < T(0)); }

} // namespace ig

#endif // IG_MATH_KERNEL_H
