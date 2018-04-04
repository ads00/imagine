/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_SIMULATION_BASE_SIGNAL_H
#define IG_SIMULATION_BASE_SIGNAL_H

#include "imagine/simulation/world/data/bridge.h"

namespace ig {

struct base_signal {
  virtual float resample(float frequency) = 0;
};

} // namespace ig

#endif // IG_SIMULATION_BASE_SIGNAL_H
