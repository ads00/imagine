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

#ifndef COMPUTE_H
#define COMPUTE_H

#include "imagine.h"

#include <cmath>
#include <limits>
#include <complex>

namespace ig
{

/* template variables not supported by VS2015 yet

template <typename T> constexpr T pi          = 3.141592653589793;
template <typename T> constexpr T half_pi     = 1.570796326794897;
template <typename T> constexpr T sqrt_two_pi = 2.506628274631000;
template <typename T> constexpr T tau         = 6.283185307179586;
template <typename T> constexpr T elog        = 2.718281828459045;
template <typename T> constexpr T sqrt2       = 1.414213562373095;
template <typename T> constexpr T golden      = 1.618033988749894;
template <typename T> constexpr T omega       = 0.567143290409783;

// Mathematical constants
template <typename T> constexpr T euler   = 2.718281828459045;
template <typename T> constexpr T apery   = 1.202056903159594;
template <typename T> constexpr T conway  = 1.303577269034296;

// Physical constants
template <typename T> constexpr T light_speed = 299792458;       // speed of light in vaccum in m.s^-1
template <typename T> constexpr T atomic_mass = 1.660538921e-27; // atomic mass constant in kg
template <typename T> constexpr T gravity     = 6.67384e-11;     // Newtonian constant of gravitation in m^3.kg^-1.s^-2
template <typename T> constexpr T planck      = 6.62606957e-34;  // Planck constant in J.s
template <typename T> constexpr T boltzmann   = 1.3806488e-23;   // Boltzmann constant in J.K-1
template <typename T> constexpr T avogadro    = 6.02214129e23;   // Avogadro's number in mol-1

template <typename T> constexpr T NaN = std::numeric_limits<T>::quiet_NaN();
template <typename T> constexpr T inf = std::numeric_limits<T>::infinity();
template <typename T> constexpr T eps = std::numeric_limits<T>::epsilon();
*/

template <typename T> constexpr T pi();
template <typename T> constexpr T two_pi();
template <typename T> constexpr T sqrt_two_pi();
template <typename T> constexpr T ln_sqrt_two_pi();

template <typename T> constexpr T euler();

template <typename T> constexpr T eps();
template <typename T> constexpr T inf();

template <> constexpr float pi<float>() { return 3.141592654f; }
template <> constexpr float two_pi<float>() { return 6.283185307f; }
template <> constexpr float sqrt_two_pi<float>() { return 2.506628274f; }
template <> constexpr float ln_sqrt_two_pi<float>() { return 0.918938533f; }

template <> constexpr float euler<float>() { return 2.718281828f; }

template <> constexpr float eps<float>() { return std::numeric_limits<float>::epsilon(); }
template <> constexpr float inf<float>() { return std::numeric_limits<float>::infinity(); }

template <> constexpr double pi<double>() { return 3.141592653589793; }
template <> constexpr double two_pi<double>() { return 6.283185307179586; }
template <> constexpr double sqrt_two_pi<double>() { return 2.506628274631000; }
template <> constexpr double ln_sqrt_two_pi<double>() { return 0.918938533204672; }

template <> constexpr double euler<double>() { return 2.718281828459045; }

template <> constexpr double eps<double>() { return std::numeric_limits<double>::epsilon(); }
template <> constexpr double inf<double>() { return std::numeric_limits<double>::infinity(); }

template <typename T>
constexpr int sign(T val) { return (T(0) < val) - (val < T(0)); }

} // namespace ig

constexpr double operator "" _deg(long double val) { return val * 180.0 / ig::pi<double>(); }
constexpr double operator "" _rad(long double val) { return val * ig::pi<double>() / 180.0; }

#endif // COMPUTE_H
