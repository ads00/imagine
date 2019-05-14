/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_RK_H
#define IG_MATH_RK_H

#include "imagine/math/basis.h"

namespace ig {

template 
< size_t Stages, 
  size_t Order,
  size_t Error,
  typename Method >
class rk : public Method {
public:
  explicit rk(float rtol = 1e-4f, float atol = 1e-6f)
    : rtol_{rtol}
    , atol_{atol} {}

  auto order () const { return Order; }
  auto stages() const { return Stages; }

  template
  < typename System, 
    typename State >
  auto init(
    System&& s, 
    State&   y,
    float t) const {

    // ERK IVP
    // use interpolant
    struct ivp_ 
    {
      State fs;
    };
    return ivp_{s.eval(t, y)};
  }

  template
  < typename System, 
    typename State,
    typename Ivp >
  auto step(
    System&& s, 
    State&   y,
    Ivp&     p,
    float& dt,
    float t) const {

    auto ev = 0;
    bool accept = false;

    while (!accept) {
      auto [ec, yn, fn] = rk_step(s, y, p.fs, dt, t);
      auto en = *std::max_element(ec.begin(), ec.end(), [](auto lhs, auto rhs) { return std::abs(lhs) < std::abs(rhs); });

      auto norm = std::inner_product(
        y.begin(),
        y.end(),
        yn.begin(),
        en,
        [ ](auto lhs, auto rhs) { return lhs + rhs * rhs; },
        [&](auto lhs, auto rhs) { return en / (atol_ + std::max(std::abs(lhs), std::abs(rhs)) * rtol_); });
      ev += Stages;

      auto ler = std::sqrt(norm);
      auto cor = std::pow(ler, -1.f / (Error + 1));
      if (ler < 1) {
        dt *= std::min(
          10.f, 
          std::max(
            1.f, 
            0.9f * cor));
        accept = true;

        y    = std::move(yn);
        p.fs = std::move(fn);
      } else {
        dt *= std::max(
          0.2f, 
          0.9f * cor);
      }
    }

    return ev;
  }

  template
  < typename System, 
    typename State >
  auto rk_step(
    System&& s, 
    State&   y,
    State&   f,
    float dt,
    float t) const {

    auto& c = nodes;
    auto& a = runge_kutta_matrix;
    auto& e = weights;
    std::array<State, Stages> K{f};

    for (size_t i = 1, j = 0; i < Stages; ++i) {
      K[i] = s.eval(
        t + dt * c[i - 1],
        y + dt * std::inner_product(
          a.begin() + j, 
          a.begin() + j + i, 
          K.begin(), 
          State{}));
      j += i;
    }

    auto yn = y + 
      std::inner_product(
        e.begin(), 
        e.begin() + Stages, 
        K.begin(), 
        State{}) * dt;
    auto fn = s.eval(t + dt, yn);

    auto err = 
      std::inner_product(
        K.begin(), 
        K.end(), 
        e.begin() + Stages, 
        State{e.back() * fn}) * dt;

    return std::make_tuple(
      err, 
      yn, 
      fn);
  }

protected:
  float rtol_, atol_;

private:
  /*
  Butcher tableau extraction, from
  J.C. Butcher (1963). "Coefficients for the study of Runge-Kutta integration processes". Journal of the Australian Mathematical Society
  */
  using Method::nodes;
  using Method::runge_kutta_matrix;
  using Method::weights;
};

struct rk23_c {
  static constexpr
  std::array nodes
  {
    0.5f,
    0.75f
  };

  static constexpr
  std::array runge_kutta_matrix
  {
    0.5f,
    0.f, 0.75f
  };

  static constexpr
  std::array weights
  {
     0.22222222222222f,
     0.33333333333333f,
     0.44444444444444f,
     0.06944444444444f,
    -0.08333333333333f,
    -0.11111111111111f,
     0.125f
  };
};
/*
Explicit Runge-Kutta method of order 3(2) from
P. Bogacki, L. F. Shampine (1989). "A 3(2) Pair of Runge-Kutta Formulas". Applied Mathematics Letters
*/
using rk23 = 
rk
< 3, 
  3, 
  2,
  rk23_c >;

struct rk45_c {
  static constexpr
  std::array nodes
  {
    0.161f,
    0.327f,
    0.9f,
    0.98002554090451f,
    1.f
  };

  static constexpr
  std::array runge_kutta_matrix
  {
     0.161f,
    -0.00848065549236f,   0.33548065549236f,
     2.89715305710549f,  -6.35944848997507f, 4.36229543286958f,
     5.32586482843926f, -11.74888356406283f, 7.49553934288984f, -0.09249506636175f,
     5.86145544294642f, -12.92096931784711f, 8.15936789857616f, -0.07158497328140f, -0.02826905039407f
  };

  static constexpr
  std::array weights
  {
     0.09646076681806f,
     0.01f,
     0.47988965041450f,
     1.37900857410374f,
    -3.29006951543608f,
     2.32471052409977f,
    -0.00178001105223f,
    -0.00081643445966f,
     0.00788087801026f,
    -0.14471100717326f,
     0.58235716545255f,
    -0.45808210592919f,
     0.01515151515151f
  };
};
/*
Explicit Runge-Kutta method of order 5(4) from
C. Tsitouras (2011). "Runge–Kutta pairs of order 5(4) satisfying only the first column simplifying assumption". Computers & Mathematics with Applications
*/
using rk45 = 
rk
< 6, 
  5, 
  4, 
  rk45_c >;

} // namespace ig

#endif // IG_MATH_RK_H
