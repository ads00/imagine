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

#ifndef IG_MATH_UNIT_H
#define IG_MATH_UNIT_H

#include "imagine/math/basis.h"

namespace ig {

template 
<int32_t Length,
 int32_t Mass,
 int32_t Time,
 int32_t Current     = 0,
 int32_t Temperature = 0,
 int32_t Amount      = 0,
 int32_t Intensity   = 0> 
struct dimensions {
  enum {
    length = Length, 
    mass   = Mass, 
    time   = Time,
    current = Current, temperature = Temperature, amount = Amount, intensity = Intensity };
}; using dimensionless = dimensions< 0, 0, 0, 0, 0, 0, 0 >;
constexpr 
struct magnitude_tag_t {} magnitude_tag{};

template <typename Dims, typename T>
class unit {
public:
  using type = T;
  using dimension_type = Dims;
  
  constexpr unit() = default;
  constexpr explicit unit(magnitude_tag_t, type x) : value{x} {}

  template <typename O>
  constexpr explicit unit(const unit<Dims, O>& o)
    : value{o} {}
  template <typename S>
  constexpr auto operator()(S x) const { return unit(magnitude_tag, value * x); }

  constexpr operator T() const { return value; }

  constexpr auto magn() const 
  { return value; }
  constexpr auto dims() const 
  { return dimension_type{}; }

protected:
  type value;
};

template <typename Fn, typename Dx, typename Dy, typename T>
constexpr auto apply_dims(const unit<Dx, T>& lhs, const unit<Dy, T>& rhs, T&& x) {
  constexpr Fn 
    fx;
  return unit<
    dimensions<fx(Dx::length, Dy::length),
               fx(Dx::mass, Dy::mass),
               fx(Dx::time, Dy::time),
               fx(Dx::current, Dy::current),
               fx(Dx::temperature, Dy::temperature),
               fx(Dx::amount, Dy::amount),
               fx(Dx::intensity, Dy::intensity)>, T>(magnitude_tag, std::forward<T>(x));
}

template <typename Dims, typename T>
constexpr auto operator+(const unit<Dims, T>& lhs, const unit<Dims, T>& rhs) 
{ return unit<Dims, T>{magnitude_tag, lhs.magn() + rhs.magn()}; }

template <typename Dims, typename T>
constexpr auto operator-(const unit<Dims, T>& lhs, const unit<Dims, T>& rhs) 
{ return unit<Dims, T>{magnitude_tag, lhs.magn() - rhs.magn()}; }

template <typename DX, typename DY, typename T>
constexpr auto operator*(const unit<DX, T>& lhs, const unit<DY, T>& rhs) 
{ return apply_dims< std::plus<int32_t> > (lhs, rhs, lhs.magn() * rhs.magn()); }

template <typename DX, typename DY, typename T>
constexpr auto operator/(const unit<DX, T>& lhs, const unit<DY, T>& rhs) 
{ return apply_dims< std::minus<int32_t> >(lhs, rhs, lhs.magn() / rhs.magn()); }

template <typename Dims, typename T>
constexpr auto operator+(const unit<Dims, T>& q, T scalar) 
{ return unit<Dims, T>(magnitude_tag, q.magn() + scalar); }

template <typename Dims, typename T>
constexpr auto operator+(T scalar, const unit<Dims, T>& q)
{ return unit<Dims, T>(magnitude_tag, scalar + q.magn()); }

template <typename Dims, typename T>
constexpr auto operator-(const unit<Dims, T>& q, T scalar) 
{ return unit<Dims, T>(magnitude_tag, q.magn() - scalar); }

template <typename Dims, typename T>
constexpr auto operator-(T scalar, const unit<Dims, T>& q)
{ return unit<Dims, T>(magnitude_tag, scalar - q.magn()); }

template <typename Dims, typename T, typename S>
constexpr auto operator*(const unit<Dims, T>& q, S scalar) 
{ return unit<Dims, T>(magnitude_tag, q.magn() * scalar); }

template <typename Dims, typename T>
constexpr auto operator*(T scalar, const unit<Dims, T>& q)
{ return unit<Dims, T>(magnitude_tag, scalar * q.magn()); }

template <typename Dims, typename T>
constexpr auto operator/(const unit<Dims, T>& q, T scalar) 
{ return unit<Dims, T>(magnitude_tag, q.magn() / scalar); }

template <typename Dims, typename T>
constexpr auto operator/(T scalar, const unit<Dims, T>& q)
{ return apply_dims< std::minus<int32_t> >(unit<dimensionless, T>{}, q, scalar / q.magn()); }

// SI base units
using length_d      = dimensions< 1, 0, 0, 0, 0, 0, 0 >;
using mass_d        = dimensions< 0, 1, 0, 0, 0, 0, 0 >;
using time_d        = dimensions< 0, 0, 1, 0, 0, 0, 0 >;
using current_d     = dimensions< 0, 0, 0, 1, 0, 0, 0 >;
using temperature_d = dimensions< 0, 0, 0, 0, 1, 0, 0 >;
using amount_d      = dimensions< 0, 0, 0, 0, 0, 1, 0 >;
using intensity_d   = dimensions< 0, 0, 0, 0, 0, 0, 1 >;

// SI derived units 
using absorbed_dose_d             = dimensions<  2,  0, -2 >;
using absorbed_dose_rate_d        = dimensions<  2,  0, -3 >;
using acceleration_d              = dimensions<  1,  0, -2 >;
using activity_of_a_nuclide_d     = dimensions<  0,  0, -1 >;
using angular_velocity_d          = dimensions<  0,  0, -1 >;
using angular_acceleration_d      = dimensions<  0,  0, -2 >;
using area_d                      = dimensions<  2,  0,  0 >;
using capacitance_d               = dimensions< -2, -1,  4,  2 >;
using catalytic_activity_d        = dimensions<  0,  0, -1,  0,  0,  1 >;
using concentration_d             = dimensions< -3,  0,  0,  0,  0,  1 >;
using current_density_d           = dimensions< -2,  0,  0,  1 >;
using dose_equivalent_d           = dimensions<  2,  0, -2 >;
using dynamic_viscosity_d         = dimensions< -1,  1, -1 >;
using electric_charge_d           = dimensions<  0,  0,  1,  1 >;
using electric_charge_density_d   = dimensions< -3,  0,  1,  1 >;
using electric_conductance_d      = dimensions< -2, -1,  3,  2 >;
using electric_field_strenth_d    = dimensions<  1,  1, -3, -1 >;
using electric_flux_density_d     = dimensions< -2,  0,  1,  1 >;
using electric_potential_d        = dimensions<  2,  1, -3, -1 >;
using electric_resistance_d       = dimensions<  2,  1, -3, -2 >;
using energy_d                    = dimensions<  2,  1, -2 >;
using energy_density_d            = dimensions< -1,  1, -2 >;
using exposure_d                  = dimensions<  0, -1,  1,  1 >;
using force_d                     = dimensions<  1,  1, -2 >;
using frequency_d                 = dimensions<  0,  0, -1 >;
using heat_capacity_d             = dimensions<  2,  1, -2,  0, -1 >;
using heat_density_d              = dimensions<  0,  1, -2 >;
using heat_density_flow_rate_d    = dimensions<  0,  1, -3 >;
using heat_flow_rate_d            = dimensions<  2,  1, -3 >;
using heat_flux_density_d         = dimensions<  0,  1, -3 >;
using heat_transfer_coefficient_d = dimensions<  0,  1, -3,  0, -1 >;
using illuminance_d               = dimensions< -2,  0,  0,  0,  0,  0,  1 >;
using inductance_d                = dimensions<  2,  1, -2, -2 >;
using irradiance_d                = dimensions<  0,  1, -3 >;
using kinematic_viscosity_d       = dimensions<  2,  0, -1 >;
using luminance_d                 = dimensions< -2,  0,  0,  0,  0,  0,  1 >;
using luminous_flux_d             = dimensions<  0,  0,  0,  0,  0,  0,  1 >;
using magnetic_field_strength_d   = dimensions< -1,  0,  0,  1 >;
using magnetic_flux_d             = dimensions<  2,  1, -2, -1 >;
using magnetic_flux_density_d     = dimensions<  0,  1, -2, -1 >;
using magnetic_permeability_d     = dimensions<  1,  1, -2, -2 >;
using mass_density_d              = dimensions< -3,  1,  0 >;
using mass_flow_rate_d            = dimensions<  0,  1, -1 >;
using molar_energy_d              = dimensions<  2,  1, -2,  0,  0, -1 >;
using molar_entropy_d             = dimensions<  2,  1, -2, -1,  0, -1 >;
using moment_of_force_d           = dimensions<  2,  1, -2 >;
using permittivity_d              = dimensions< -3, -1,  4,  2 >;
using power_d                     = dimensions<  2,  1, -3 >;
using pressure_d                  = dimensions< -1,  1, -2 >;
using radiance_d                  = dimensions<  0,  1, -3 >;
using radiant_intensity_d         = dimensions<  2,  1, -3 >;
using speed_d                     = dimensions<  1,  0, -1 >;
using specific_energy_d           = dimensions<  2,  0, -2 >;
using specific_heat_capacity_d    = dimensions<  2,  0, -2,  0, -1 >;
using specific_volume_d           = dimensions<  3, -1,  0 >;
using substance_permeability_d    = dimensions< -1,  0,  1 >;
using surface_tension_d           = dimensions<  0,  1, -2 >;
using thermal_conductivity_d      = dimensions<  1,  1, -3,  0, -1 >;
using thermal_diffusivity_d       = dimensions<  2,  0, -1 >;
using thermal_insulance_d         = dimensions<  0, -1,  3,  0,  1 >;
using thermal_resistance_d        = dimensions< -2, -1,  3,  0,  1 >;
using thermal_resistivity_d       = dimensions< -1, -1,  3,  0,  1 >;
using torque_d                    = dimensions<  2,  1, -2 >;
using volume_d                    = dimensions<  3,  0,  0 >;
using volume_flow_rate_d          = dimensions<  3,  0, -1 >;
using wave_number_d               = dimensions< -1,  0,  0 >;

// The seven SI base units. 
template <typename T = double> constexpr unit<length_d, T>      metre    { magnitude_tag, T(1) };
template <typename T = double> constexpr unit<mass_d, T>        kilogram { magnitude_tag, T(1) };
template <typename T = double> constexpr unit<time_d, T>        second   { magnitude_tag, T(1) };
template <typename T = double> constexpr unit<current_d, T>     ampere   { magnitude_tag, T(1) };
template <typename T = double> constexpr unit<temperature_d, T> kelvin   { magnitude_tag, T(1) };
template <typename T = double> constexpr unit<amount_d, T>      mole     { magnitude_tag, T(1) };
template <typename T = double> constexpr unit<intensity_d, T>   candela  { magnitude_tag, T(1) };

// The derived SI units, as specified in SP811.
template <typename T = double> constexpr T                                radian    { T(1) };
template <typename T = double> constexpr T                                steradian { T(1) };
template <typename T = double> constexpr unit<frequency_d, T>             hertz     { T(1) / second<T> };
template <typename T = double> constexpr unit<force_d, T>                 newton    { metre<T> * kilogram<T> / (second<T> * second<T>) };
template <typename T = double> constexpr unit<pressure_d, T>              pascal    { newton<T> / (metre<T> * metre<T>) };
template <typename T = double> constexpr unit<energy_d, T>                joule     { newton<T> * metre<T> };
template <typename T = double> constexpr unit<power_d, T>                 watt      { joule<T> / second<T> };
template <typename T = double> constexpr unit<electric_charge_d, T>       coulomb   { second<T> * ampere<T> };
template <typename T = double> constexpr unit<electric_potential_d, T>    volt      { watt<T> / ampere<T> };
template <typename T = double> constexpr unit<capacitance_d, T>           farad     { coulomb<T> / volt<T> };
template <typename T = double> constexpr unit<electric_resistance_d, T>   ohm       { volt<T> / ampere<T> };
template <typename T = double> constexpr unit<electric_conductance_d, T>  siemens   { ampere<T> / volt<T> };
template <typename T = double> constexpr unit<magnetic_flux_d, T>         weber     { volt<T> * second<T> };
template <typename T = double> constexpr unit<magnetic_flux_density_d, T> tesla     { weber<T> / (metre<T> * metre<T>) };
template <typename T = double> constexpr unit<inductance_d, T>            henry     { weber<T> / ampere<T> };
template <typename T = double> constexpr unit<luminous_flux_d, T>         lumen     { candela<T> * steradian<T> };
template <typename T = double> constexpr unit<illuminance_d, T>           lux       { lumen<T> / metre<T> / metre<T> };
template <typename T = double> constexpr unit<activity_of_a_nuclide_d, T> becquerel { T(1) / second<T> };
template <typename T = double> constexpr unit<absorbed_dose_d, T>         gray      { joule<T> / kilogram<T> };
template <typename T = double> constexpr unit<dose_equivalent_d, T>       sievert   { joule<T> / kilogram<T> };
template <typename T = double> constexpr unit<catalytic_activity_d, T>    katal     { T(1) / second<T> * mole<T> };

template <typename T = double> constexpr unit<activity_of_a_nuclide_d, T> curie    { T(3.7e+10) * becquerel<T> };
template <typename T = double> constexpr T                                degree   { pi<T> / T(180) };
template <typename T = double> constexpr unit<exposure_d, T>              roentgen { T(2.58e-4) * coulomb<T> / kilogram<T> };

constexpr auto operator ""_m(long double x)   { return metre<>(x); }     constexpr auto operator ""_m(unsigned long long x)   { return metre<>(x); }
constexpr auto operator ""_kg(long double x)  { return kilogram<>(x); }  constexpr auto operator ""_kg(unsigned long long x)  { return kilogram<>(x); }
constexpr auto operator ""_s(long double x)   { return second<>(x); }    constexpr auto operator ""_s(unsigned long long x)   { return second<>(x); }
constexpr auto operator ""_A(long double x)   { return ampere<>(x); }    constexpr auto operator ""_A(unsigned long long x)   { return ampere<>(x); }
constexpr auto operator ""_K(long double x)   { return kelvin<>(x); }    constexpr auto operator ""_K(unsigned long long x)   { return kelvin<>(x); }
constexpr auto operator ""_mol(long double x) { return mole<>(x); }      constexpr auto operator ""_mol(unsigned long long x) { return mole<>(x); }
constexpr auto operator ""_cd(long double x)  { return candela<>(x); }   constexpr auto operator ""_cd(unsigned long long x)  { return candela<>(x); }
constexpr auto operator ""_Hz(long double x)  { return hertz<>(x); }     constexpr auto operator ""_Hz(unsigned long long x)  { return hertz<>(x); }
constexpr auto operator ""_N(long double x)   { return newton<>(x); }    constexpr auto operator ""_N(unsigned long long x)   { return newton<>(x); }
constexpr auto operator ""_Pa(long double x)  { return pascal<>(x); }    constexpr auto operator ""_Pa(unsigned long long x)  { return pascal<>(x); }
constexpr auto operator ""_J(long double x)   { return joule<>(x); }     constexpr auto operator ""_J(unsigned long long x)   { return joule<>(x); }
constexpr auto operator ""_W(long double x)   { return watt<>(x); }      constexpr auto operator ""_W(unsigned long long x)   { return watt<>(x); }
constexpr auto operator ""_C(long double x)   { return coulomb<>(x); }   constexpr auto operator ""_C(unsigned long long x)   { return coulomb<>(x); }
constexpr auto operator ""_V(long double x)   { return volt<>(x); }      constexpr auto operator ""_V(unsigned long long x)   { return volt<>(x); }
constexpr auto operator ""_F(long double x)   { return farad<>(x); }     constexpr auto operator ""_F(unsigned long long x)   { return farad<>(x); }
constexpr auto operator ""_ohm(long double x) { return ohm<>(x); }       constexpr auto operator ""_ohm(unsigned long long x) { return ohm<>(x); }
constexpr auto operator ""_S(long double x)   { return siemens<>(x); }   constexpr auto operator ""_S(unsigned long long x)   { return siemens<>(x); }
constexpr auto operator ""_Wb(long double x)  { return weber<>(x); }     constexpr auto operator ""_Wb(unsigned long long x)  { return weber<>(x); }
constexpr auto operator ""_T(long double x)   { return tesla<>(x); }     constexpr auto operator ""_T(unsigned long long x)   { return tesla<>(x); }
constexpr auto operator ""_H(long double x)   { return henry<>(x); }     constexpr auto operator ""_H(unsigned long long x)   { return henry<>(x); }
constexpr auto operator ""_lm(long double x)  { return lumen<>(x); }     constexpr auto operator ""_lm(unsigned long long x)  { return lumen<>(x); }
constexpr auto operator ""_lx(long double x)  { return lux<>(x); }       constexpr auto operator ""_lx(unsigned long long x)  { return lux<>(x); }
constexpr auto operator ""_Bq(long double x)  { return becquerel<>(x); } constexpr auto operator ""_Bq(unsigned long long x)  { return becquerel<>(x); }
constexpr auto operator ""_Gy(long double x)  { return gray<>(x); }      constexpr auto operator ""_Gy(unsigned long long x)  { return gray<>(x); }
constexpr auto operator ""_Sv(long double x)  { return sievert<>(x); }   constexpr auto operator ""_Sv(unsigned long long x)  { return sievert<>(x); }
constexpr auto operator ""_kat(long double x) { return katal<>(x); }     constexpr auto operator ""_kat(unsigned long long x) { return katal<>(x); }
constexpr auto operator ""_Ci(long double x)  { return curie<>(x); }     constexpr auto operator ""_Ci(unsigned long long x)  { return curie<>(x); }
constexpr auto operator ""_R(long double x)   { return roentgen<>(x); }  constexpr auto operator ""_R(unsigned long long x)   { return roentgen<>(x); }

constexpr double operator ""_rad(long double x) { return radian<> * x; }    constexpr double operator ""_rad(unsigned long long x) { return radian<> * x; }
constexpr double operator ""_sr(long double x)  { return steradian<> * x; } constexpr double operator ""_sr(unsigned long long x)  { return steradian<> * x; }
constexpr double operator ""_deg(long double x) { return degree<> * x; }    constexpr double operator ""_deg(unsigned long long x) { return degree<> * x; }

} // namespace ig

#endif // IG_MATH_UNIT_H
