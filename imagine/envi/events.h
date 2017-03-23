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

#ifndef IG_ENVI_EVENTS_H
#define IG_ENVI_EVENTS_H

#include "imagine/envi/input_keyboard.h"
#include "imagine/envi/input_mouse.h"

#include <functional>

namespace ig {

// keyboard events
struct event_keyboard {
  enum type_t {
    none,
    pressed, released,
  } type;

  keyboard::modifiers modifiers;
  keyboard::key key;
  uint32_t vk;
};

// mouse events
struct event_mouse {
  enum type_t {
    none,
    pressed, released, dbl_clicked,
    moved, entered, leaved, wheeled,
  } type;

  struct event_click { mouse::button button; };
  struct event_move  { int32_t dx, dy; };
  struct event_wheel { float delta; };
  
  keyboard::modifiers modifiers;
  mouse::buttons buttons;
  int32_t x, y;

  union {
    event_click click;
    event_move  move;
    event_wheel wheel;
  };
};

// status events
struct event_status {
  enum type_t {
    none,
    moved, resized, closed
  } type;
};

class ig_api events {
public:
  template <typename T> using func_type = std::function<void(const T&)>;
  events() = default;

  template <typename T>
  void process(const T& arg) const {
    auto& fn = std::get< func_type<T> >(handlers_);
    if (fn) {
      fn(arg);
    }
  }

  void keyboard(const func_type<event_keyboard>& fn) { std::get< std::decay_t<decltype(fn)> >(handlers_) = fn; }
  void mouse   (const func_type<event_mouse>& fn)    { std::get< std::decay_t<decltype(fn)> >(handlers_) = fn; }
  void status  (const func_type<event_status>& fn)   { std::get< std::decay_t<decltype(fn)> >(handlers_) = fn; }

private:
  std::tuple< func_type<event_keyboard>, func_type<event_mouse>,
              func_type<event_status> > handlers_;
};

} // namespace ig

#endif // IG_ENVI_EVENTS_H
