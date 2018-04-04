/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_BASIS_H
#define IG_MATH_BASIS_H

#include "imagine/ig.h"

#include <cmath>
#include <numeric>
#include <sstream>

namespace ig {

// Mathematical constants
template <typename Arithmetic> constexpr Arithmetic pi         = Arithmetic(3.141592653589793);
template <typename Arithmetic> constexpr Arithmetic two_pi     = Arithmetic(6.283185307179586);
template <typename Arithmetic> constexpr Arithmetic e          = Arithmetic(2.718281828459045);
template <typename Arithmetic> constexpr Arithmetic sqrt2      = Arithmetic(1.414213562373095);
template <typename Arithmetic> constexpr Arithmetic sqrt3      = Arithmetic(1.732050807568877);
template <typename Arithmetic> constexpr Arithmetic euler      = Arithmetic(0.577215664901532);
template <typename Arithmetic> constexpr Arithmetic golden     = Arithmetic(1.618033988749894);
template <typename Arithmetic> constexpr Arithmetic mertens    = Arithmetic(0.261497212847642);
template <typename Arithmetic> constexpr Arithmetic bernstein  = Arithmetic(0.280169499023869);
template <typename Arithmetic> constexpr Arithmetic omega      = Arithmetic(0.567143290409783);
template <typename Arithmetic> constexpr Arithmetic cahen      = Arithmetic(0.643410546288338);
template <typename Arithmetic> constexpr Arithmetic laplace    = Arithmetic(0.662743419349181);
template <typename Arithmetic> constexpr Arithmetic landau     = Arithmetic(0.764223653589220);
template <typename Arithmetic> constexpr Arithmetic catalan    = Arithmetic(0.915965594177219);
template <typename Arithmetic> constexpr Arithmetic lengyel    = Arithmetic(1.098685805525187);
template <typename Arithmetic> constexpr Arithmetic apery      = Arithmetic(1.202056903159594);
template <typename Arithmetic> constexpr Arithmetic conway     = Arithmetic(1.303577269034296);
template <typename Arithmetic> constexpr Arithmetic mills      = Arithmetic(1.306377883863080);
template <typename Arithmetic> constexpr Arithmetic plastic    = Arithmetic(1.324717957244746);
template <typename Arithmetic> constexpr Arithmetic soldner    = Arithmetic(1.451369234883381);
template <typename Arithmetic> constexpr Arithmetic backhouse  = Arithmetic(1.456074948582689);
template <typename Arithmetic> constexpr Arithmetic lieb       = Arithmetic(1.539600717839002);
template <typename Arithmetic> constexpr Arithmetic niven      = Arithmetic(1.705211140105367);
template <typename Arithmetic> constexpr Arithmetic parabolic  = Arithmetic(2.295587149392638);
template <typename Arithmetic> constexpr Arithmetic sierpinski = Arithmetic(2.584981759579253);
template <typename Arithmetic> constexpr Arithmetic khinchin   = Arithmetic(2.685452001065306);
template <typename Arithmetic> constexpr Arithmetic levy       = Arithmetic(3.275822918721811);

// Physical constants
template <typename Arithmetic> constexpr Arithmetic light_speed = Arithmetic(299792458);       // speed of light in vaccum in m.s^-1
template <typename Arithmetic> constexpr Arithmetic atomic_mass = Arithmetic(1.660538921e-27); // atomic mass constant in kg
template <typename Arithmetic> constexpr Arithmetic gravity     = Arithmetic(6.67384e-11);     // Newtonian constant of gravitation in m^3.kg^-1.s^-2
template <typename Arithmetic> constexpr Arithmetic planck      = Arithmetic(6.62606957e-34);  // Planck constant in J.s
template <typename Arithmetic> constexpr Arithmetic boltzmann   = Arithmetic(1.3806488e-23);   // Boltzmann constant in J.K-1
template <typename Arithmetic> constexpr Arithmetic avogadro    = Arithmetic(6.02214129e+23);  // Avogadro's number in mol-1

template <typename Arithmetic>
constexpr auto sign(Arithmetic x) { return (x > 0) - (x < 0); }

template <typename Arithmetic>
constexpr auto align(Arithmetic x, Arithmetic alignment) { return (x + alignment - 1) &~(alignment - 1); }

} // namespace ig

#endif // IG_MATH_BASIS_H
