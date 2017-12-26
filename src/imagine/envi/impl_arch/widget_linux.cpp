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

#include "imagine/envi/impl_arch/widget_impl.h"
#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/impl_arch/cursor_impl.h"
#include "imagine/envi/impl_arch/interface_keyboard_impl.h"
#include "imagine/envi/impl_arch/interface_mouse_impl.h"

namespace ig   {
namespace impl {

widget_impl::widget_impl(const widget& ref)
  : ref_{ref}
  , styles_{widget_style::none}
  , visibility_{widget_visibility::hidden}
  , mouse_tracked_{false} {

  /// wayland: TODO
}

widget_impl::widget_impl(const widget& ref, widget_styles styles, const std::string& caption, uint32_t w, uint32_t h)
  : ref_{ref}
  , styles_{styles}
  , visibility_{widget_visibility::windowed}
  , caption_{caption}
  , mouse_tracked_{false} {

  /// wayland: TODO
}

} // namespace impl

void widget::close() {
  /// wayland: TODO
}

void widget::show() {
  /// wayland: TODO
}

void widget::hide() {
  /// wayland: TODO
}

void widget::focus() {
  /// wayland: TODO
}

void widget::resize(uint32_t width, uint32_t height) {
  /// wayland: TODO
}

void widget::move(int32_t x, int32_t y) {
  /// wayland: TODO
}

bool widget::opened() const
{ return false; }

bool widget::minimized() const
{ return false; }

bool widget::maximized() const
{ return false; }

bool widget::visible() const
{ return false; }

auto widget::get_handle() const -> handle*
{ return nullptr; }

void widget::set_fullscreen(bool fullscreen) {
  /// wayland: TODO
}

void widget::set_caption(const std::string& caption) {
  /// wayland: TODO
}

void widget::set_parent(const widget* parent) {
  /// wayland: TODO
}

} // namespace ig
