/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_INTERFACE_KEYBOARD_IMPL_H
#define IG_ENVI_INTERFACE_KEYBOARD_IMPL_H

#include "imagine/envi/interface_keyboard.h"
#include "imagine/envi/impl_arch/config_impl.h"

namespace ig       {
namespace keyboard {
namespace impl     {

#if defined(IG_WIN)
auto get_modifiers()        -> modifiers;
auto get_key(WPARAM wparam) -> key;
#endif

} // namespace impl
} // namespace keyboard
} // namespace ig

#endif // IG_ENVI_INTERFACE_KEYBOARD_IMPL_H
