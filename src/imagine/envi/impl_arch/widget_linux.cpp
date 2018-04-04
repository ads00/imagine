/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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
}

widget_impl::widget_impl(const widget& ref, widget_styles styles, const std::string& caption, uint32_t w, uint32_t h)
  : ref_{ref}
  , styles_{styles}
  , visibility_{widget_visibility::windowed}
  , caption_{caption}
  , mouse_tracked_{false} {
}

} // namespace impl

void widget::close() {
}

void widget::show() {
}

void widget::hide() {
}

void widget::focus() {
}

void widget::resize(uint32_t width, uint32_t height) {
}

void widget::move(int32_t x, int32_t y) {
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
}

void widget::set_caption(const std::string& caption) {
}

void widget::set_parent(const widget* parent) {
}

} // namespace ig
