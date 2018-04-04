/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/cursor_impl.h"
#include "imagine/envi/impl_arch/widget_impl.h"

namespace ig   {
namespace impl {

cursor_impl::cursor_impl(cursor_shape shape, int32_t x, int32_t y)
  : shape_{shape}
  , x_{x}
  , y_{y} {
}

} // namespace impl

void cursor::refresh() const {
}

void cursor::clip(const widget& widget) {
}

void cursor::move(int32_t x, int32_t y, const widget* widget) {
}

} // namespace ig
