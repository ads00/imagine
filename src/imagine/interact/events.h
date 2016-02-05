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

#ifndef INTERACT_EVENTS_H
#define INTERACT_EVENTS_H

#include "imagine/interact/keyboard.h"
#include "imagine/interact/mouse.h"
#include <functional>

namespace ig
{

// keyboard events
struct arg_keyboard
{
  enum type_t
  {
    none,
    key_press, key_release,
  } type;

  modifier_ft modifiers;
  uint32_t vk;
  key_t key;
};

// mouse events
struct arg_mouse
{
  enum type_t
  {
    none,
    mouse_down, mouse_up, mouse_dbl_click,
    mouse_move, mouse_enter, mouse_leave, mouse_wheel,
  } type;

  struct arg_click { button_t button; };
  struct arg_wheel { float delta; };

  modifier_ft modifiers;
  button_ft buttons;
  int x, y;

  union
  {
    arg_click click;
    arg_wheel wheel;
  };
};

// status events
struct arg_status
{
  enum type_t
  {
    none,
    move, resize, close
  } type;
};

class events
{
public:
  template <typename T>
  using func_t = std::function<void (const T&)>;

  constexpr events() = default;

  template <typename T>
  void process(const T& arg) const
  {
    const auto& func = std::get< func_t<T> >(funcs_);
    if (func) func(arg);
  }

  void keyboard(func_t<arg_keyboard> func) { std::get<decltype(func)>(funcs_) = func; }
  void mouse(func_t<arg_mouse> func)       { std::get<decltype(func)>(funcs_) = func; }

  void status(func_t<arg_status> func)     { std::get<decltype(func)>(funcs_) = func; }

protected:
  std::tuple< func_t<arg_keyboard>, func_t<arg_mouse>,
              func_t<arg_status> > funcs_;
};

} // namespace ig

#endif // INTERACT_EVENTS_H
