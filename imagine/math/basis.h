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

#ifndef IG_MATH_BASIS_H
#define IG_MATH_BASIS_H

#include "imagine/ig.h"

#include <cmath>
#include <sstream>

namespace ig {

// Mathematical constants
template <typename T = double> constexpr T pi         = T(3.141592653589793);
template <typename T = double> constexpr T two_pi     = T(6.283185307179586);
template <typename T = double> constexpr T e          = T(2.718281828459045);
template <typename T = double> constexpr T sqrt2      = T(1.414213562373095);
template <typename T = double> constexpr T sqrt3      = T(1.732050807568877);
template <typename T = double> constexpr T euler      = T(0.577215664901532);
template <typename T = double> constexpr T golden     = T(1.618033988749894);
template <typename T = double> constexpr T mertens    = T(0.261497212847642);
template <typename T = double> constexpr T bernstein  = T(0.280169499023869);
template <typename T = double> constexpr T omega      = T(0.567143290409783);
template <typename T = double> constexpr T cahen      = T(0.643410546288338);
template <typename T = double> constexpr T laplace    = T(0.662743419349181);
template <typename T = double> constexpr T landau     = T(0.764223653589220);
template <typename T = double> constexpr T catalan    = T(0.915965594177219);
template <typename T = double> constexpr T lengyel    = T(1.098685805525187);
template <typename T = double> constexpr T apery      = T(1.202056903159594);
template <typename T = double> constexpr T conway     = T(1.303577269034296);
template <typename T = double> constexpr T mills      = T(1.306377883863080);
template <typename T = double> constexpr T plastic    = T(1.324717957244746);
template <typename T = double> constexpr T soldner    = T(1.451369234883381);
template <typename T = double> constexpr T backhouse  = T(1.456074948582689);
template <typename T = double> constexpr T lieb       = T(1.539600717839002);
template <typename T = double> constexpr T niven      = T(1.705211140105367);
template <typename T = double> constexpr T parabolic  = T(2.295587149392638);
template <typename T = double> constexpr T sierpinski = T(2.584981759579253);
template <typename T = double> constexpr T khinchin   = T(2.685452001065306);
template <typename T = double> constexpr T levy       = T(3.275822918721811);

// Physical constants
template <typename T> constexpr T light_speed = T(299792458);       // speed of light in vaccum in m.s^-1
template <typename T> constexpr T atomic_mass = T(1.660538921e-27); // atomic mass constant in kg
template <typename T> constexpr T gravity     = T(6.67384e-11);     // Newtonian constant of gravitation in m^3.kg^-1.s^-2
template <typename T> constexpr T planck      = T(6.62606957e-34);  // Planck constant in J.s
template <typename T> constexpr T boltzmann   = T(1.3806488e-23);   // Boltzmann constant in J.K-1
template <typename T> constexpr T avogadro    = T(6.02214129e+23);  // Avogadro's number in mol-1

template <typename T>
constexpr T sign(T x) { 
  return x > 0 
    ? 1 
    : x < 0 
      ? -1 
      :  0;
}

template <typename T>
constexpr auto align(T x, T alignment) { return x + alignment - 1 &~(alignment - 1); }

} // namespace ig

#endif // IG_MATH_BASIS_H
