/*
 Copyright (c) 2015, 2016
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

#ifndef IG_ENVI_EVENTS_H
#define IG_ENVI_EVENTS_H

#include "imagine/envi/keyboard.h"
#include "imagine/envi/mouse.h"

#include <functional>

namespace ig {

// keyboard events
struct arg_keyboard {

  enum type_t {
    none,
    key_press, key_release,
  } type;

  modifier_ft modifiers;
  uint32_t vk;
  key_t key;
};

// mouse events
struct arg_mouse {

  enum type_t {
    none,
    mouse_down, mouse_up, mouse_dbl_click,
    mouse_move, mouse_enter, mouse_leave, mouse_wheel,
  } type;

  struct arg_move  { int32_t dx, dy; };
  struct arg_click { button_t button; };
  struct arg_wheel { float delta; };
  

  modifier_ft modifiers;
  button_ft buttons;
  int32_t x, y;

  union {
    arg_move  move;
    arg_click click;
    arg_wheel wheel;
  };
};

// status events
struct arg_status {

  enum type_t {
    none,
    move, resize, close
  } type;
};

class IG_API events {
public:
  template <typename T> 
  using fn_t = std::function<void (const T&)>;

  constexpr events() = default;

  template <typename T>
  void process(T&& arg) const {
    auto fn = std::get< fn_t<T> >(fns_);
    if (fn) {
      fn(std::forward<T>(arg));
    }
  }

  void keyboard(fn_t<arg_keyboard> fn) { std::get<decltype(fn)>(fns_) = fn; }
  void mouse(fn_t<arg_mouse> fn)       { std::get<decltype(fn)>(fns_) = fn; }

  void status(fn_t<arg_status> fn)     { std::get<decltype(fn)>(fns_) = fn; }

protected:
  std::tuple< fn_t<arg_keyboard>, fn_t<arg_mouse>,
              fn_t<arg_status> > fns_;
};

} // namespace ig

#endif // IG_ENVI_EVENTS_H
