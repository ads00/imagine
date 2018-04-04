/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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
