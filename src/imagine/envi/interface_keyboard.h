/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_INTERFACE_KEYBOARD_H
#define IG_ENVI_INTERFACE_KEYBOARD_H

#include "imagine/core/flags.h"

namespace ig       {
namespace keyboard {

enum class key {
  unknown = -1,

  // misc
  escape,
  tab,
  ret,
  del,
  backtab,
  backspace,
  enter,
  insert,
  cancel,
  clear,
  pause,
  space,
  page_up, page_down,
  end,
  home,
  print,
  left, up, right, down,

  // modifiers
  shift,
  control,
  alt,
  meta,
  caps_lock,
  num_lock,
  scroll_lock,

  // letters
  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,

  // function
  f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24,

  // numeric
  asterisk,
  plus,
  comma,
  minus,
  period,
  slash,
  num0, num1, num2, num3, num4, num5, num6, num7, num8, num9,

  // navigation
  select,
  yes,
  no,

  // multimedia
  back, forward,
  refresh,
  stop,
  search,
  favorites,
  home_page,
  volume_mute, volume_down, volume_up,
  media_next, media_previous, media_stop, media_play, launch_mail, launch_media, launch0, launch1,

  // others
  mode_switch,
  help,
  menu,

  printer,
  execute,
  sleep,
  play,
  zoom
};

enum class modifier : uint32_t {
  none  = 0x000,
  shift = 0x001,
  ctrl  = 0x002,
  alt   = 0x004,
  meta  = 0x008
}; using modifiers = flags<modifier>;

} // namespace keyboard
} // namespace ig

#endif // IG_ENVI_INTERFACE_KEYBOARD_H
