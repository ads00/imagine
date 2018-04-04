/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_INTERFACE_MOUSE_H
#define IG_ENVI_INTERFACE_MOUSE_H

#include "imagine/core/flags.h"

namespace ig    {
namespace mouse {

enum class button : uint32_t {
  none   = 0x00,
  left   = 0x01,
  right  = 0x02,
  middle = 0x04
}; using buttons = flags<button>;

} // namespace mouse
} // namespace ig

#endif // IG_ENVI_INTERFACE_MOUSE_H
