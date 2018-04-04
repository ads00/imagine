/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/cursor_impl.h"
#include "imagine/envi/cursor.h"

namespace ig {

cursor::cursor(cursor_shape shape)
  : native_{std::make_unique<impl::cursor_impl>(shape)} {}

cursor::~cursor() = default;

void cursor::reshape(cursor_shape shape) {
  native_.reset(new impl::cursor_impl{shape, native_->x_, native_->y_});
  refresh();
}

// Native implementations
//

// void cursor::refresh() const;
// static void cursor::clip(const widget& widget);
// static void cursor::move(int32_t x, int32_t y, const widget* widget = nullptr);

} // namespace ig
