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

#ifndef IG_ENVI_INTERFACE_H
#define IG_ENVI_INTERFACE_H

#include "imagine/envi/interface_keyboard.h"
#include "imagine/envi/interface_mouse.h"

namespace ig {

// Widget keyboard event
struct keyboard_event {
  enum type_t {
    none,
    pressed, released,
  } type;

  keyboard::modifiers modifiers;
  keyboard::key key;
  uint32_t vk;
};

// Widget mouse event
struct mouse_event {
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

// Widget status event
struct status_event {
  enum type_t {
    none,
    moved, resized, closed
  } type;
};

class IG_API interfaces {
public:
  interfaces() = default;

  template <typename Event>
  class event {
  public:
    using event_callback = std::function< void(const Event&) >;

    void operator()(const Event& ev) const
    { if (handler_) handler_(ev); }
    void operator()(const event_callback& fn) { handler_ = fn; }

  private:
    event_callback handler_;
  };

  event<status_event> status;
  event<keyboard_event> keyboard; event<mouse_event> mouse;
};

} // namespace ig

#endif // IG_ENVI_INTERFACE_H
