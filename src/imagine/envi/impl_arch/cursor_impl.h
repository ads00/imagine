/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_CURSOR_IMPL_H
#define IG_ENVI_CURSOR_IMPL_H

#include "imagine/envi/cursor.h"
#include "imagine/envi/impl_arch/config_impl.h"

namespace ig   {
namespace impl {

class cursor_impl {
public:
  cursor_impl(cursor_shape shape, int32_t x = 0, int32_t y = 0);
  ~cursor_impl() = default;

  cursor_shape shape_;
  int32_t x_, y_;

  #if defined(IG_WIN)
  HCURSOR handle_;
  #endif
};

} // namespace impl
} // namespace ig

#endif // IG_ENVI_CURSOR_IMPL_H
