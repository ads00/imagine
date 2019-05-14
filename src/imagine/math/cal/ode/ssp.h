/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_SSP_H
#define IG_MATH_SSP_H

#include "imagine/math/basis.h"

namespace ig {

template 
< size_t Stages, 
  size_t Order,
  typename Method >
class ssp : public Method {
public:
  ssp() = default;

  auto order () const { return Order; }
  auto stages() const { return Stages; }

  template
  < typename System, 
    typename State >
  auto init(
    System&& s, 
    State&   y,
    float t) const {

    struct ivp_{};
    return ivp_{};
  }

  template
  < typename System, 
    typename State,
    typename Ivp >
  auto step(
    System&& s, 
    State&   y, 
    Ivp&     p,
    float dt,
    float t) const {

    auto& [pn, qn] = y;
    for (size_t i = 0; i < Stages; ++i) 
      qn += s.eval_q(pn) * dt * coordinates[i], pn += s.eval_p(qn) * dt * momentums[i];

    return Stages * 2;
  }

private:
  /*
  Symplectic tableau extraction, from
  R. D. Ruth (1983). "A Canonical Integration Technique". Nuclear Science, IEEE Trans. on. NS-30
  */
  using Method::momentums;
  using Method::coordinates;
};

struct leapfrog_c {
  static constexpr 
  std::array momentums
  {
    0.5f,
    0.5f
  };

  static constexpr 
  std::array coordinates
  {
    0.f,
    1.f
  };
};
/*
Explicit Symplectic method of order 2 from
C. K. Birdsall, A. B. Langdon (1985). "Plasma Physics via Computer Simulations". McGraw-Hill Book Company, p. 56
*/
using leapfrog = 
ssp
< 2, 
  2,
  leapfrog_c >;

struct ssp5_c {
  static constexpr 
  std::array momentums
  {
     0.36953388878115f,
    -0.03212000426305f,
    -0.01197870102055f, 0.51263817465270f,
    -0.33494829803588f, 0.02185659474110f, 0.47501834514454f
  };

  static constexpr 
  std::array coordinates
  {
     0.11256958446835f,
     0.92380502900084f, 
    -1.36206489866978f, 0.98092653187932f, 
     0.40096296748537f, 0.34582178086474f, -0.40202099502884f
  };
};
/*
Explicit Symplectic method of order 5 from
K. Tselios, T. E. Simos (2012). "Optimized fifth order symplectic integrators for orbital problems". Revista Mexicana de Astronomía y Astrofísica
*/
using ssp5 = 
ssp
< 7,
  5,
  ssp5_c >;

struct ssp8_c {
  static constexpr 
  std::array momentums
  {
     0.13020248308889f,
     0.56116298177511f,
    -0.38947496264485f,
     0.15884190655516f,
    -0.39590389413324f,
     0.18453964097832f,
     0.25837438768632f,
     0.29501172360931f,
    -0.60550853383003f,
     0.29501172360931f,  0.25837438768632f,
     0.18453964097832f, -0.39590389413324f, 0.15884190655516f,
    -0.38947496264485f,  0.56116298177511f, 0.13020248308889f, 0.f
  };

  static constexpr 
  std::array coordinates
  {
     0.06510124154445f,
     0.34568273243200f,
     0.08584400956513f,
    -0.11531652804485f,
    -0.11853099378904f,
    -0.10568212657746f,
     0.22145701433232f,
     0.27669305564782f,
    -0.15524840511036f,
    -0.15524840511036f,  0.27669305564782f,
     0.22145701433232f, -0.10568212657746f, -0.11853099378904f,
    -0.11531652804485f,  0.08584400956513f,  0.34568273243200f, 0.06510124154445f
  };
};
/*
Explicit Symplectic method of order 8 from
W. Kahan, R-C. Li (1997). "Composition constants for raising the orders of unconventional schemes for ordinary differential equations". Mathematics of Computation
*/
using ssp8 = 
ssp
< 18,
   8,
  ssp8_c >;

} // namespace ig

#endif // IG_MATH_SSP_H
