/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_INTERFACE_MOUSE_IMPL_H
#define IG_ENVI_INTERFACE_MOUSE_IMPL_H

#include "imagine/envi/interface_mouse.h"
#include "imagine/envi/impl_arch/config_impl.h"

namespace ig    {
namespace mouse {
namespace impl  {

#if defined(IG_WIN)
auto get_buttons() -> buttons;

auto get_x(LPARAM lparam) -> int32_t;
auto get_y(LPARAM lparam) -> int32_t;
auto get_wheel_delta(WPARAM wparam) -> float;

auto track(HWND widget) -> bool;
#endif

} // namespace impl
} // namespace mouse
} // namespace ig

#endif // IG_ENVI_INTERFACE_MOUSE_IMPL_H
