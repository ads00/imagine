/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_ODE_H
#define IG_MATH_ODE_H

#include "imagine/math/basis.h"

namespace ig {

template 
< typename Integrator,
  typename System >
class ode {
public:
  explicit ode(
    const Integrator& integrator, 
    const System& system, 
    float ti, 
    float tn)
    : int_{integrator}
    , sys_{system}
    , evals_{0}
    , ti_{ti}
    , tn_{tn} {}

  auto order () const { return int_.order(); }
  auto stages() const { return int_.stages(); }
  auto evals () const { return evals_; }

  template 
  < typename State, 
    typename Observer >
  void solve(float dt, State& y, Observer&& observer) {
    assert((tn_ - ti_) * dt > 0 && "Invalid integration direction within given interval");

    auto ivp = 
      int_.init(
        sys_, 
        y, 
        ti_);
    while (std::abs(tn_ - ti_) > std::abs(dt)) {
      evals_ += int_.step(
        sys_,
        y,
        ivp,
        dt,
        ti_);
      observer(
        ti_ += dt,
        y);
    }
  }

private:
  Integrator int_;
  System     sys_;
  size_t evals_;
  float 
  ti_, 
  tn_;
};

} // namespace ig

#endif // IG_MATH_ODE_H
