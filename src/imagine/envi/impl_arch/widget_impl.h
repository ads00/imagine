/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_WIDGET_IMPL_H
#define IG_ENVI_WIDGET_IMPL_H

#include "imagine/envi/widget.h"
#include "imagine/envi/impl_arch/config_impl.h"

namespace ig   {
namespace impl {

class widget_impl {
public:
  widget_impl(const widget& ref);
  widget_impl(const widget& ref, widget_styles styles, const std::string& caption, uint32_t w, uint32_t h);
  ~widget_impl() = default;

  const widget& ref_;

  widget_styles styles_;
  widget_visibility visibility_;

  std::string caption_;
  uint32_t w_, h_;
  int32_t  x_, y_;

  bool mouse_tracked_;

  #if defined(IG_WIN)
  LRESULT internal(
    HWND hwnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam);

  DWORD wstyle_;
  HINSTANCE instance_;
  HWND handle_;
  #endif
};

} // namespace impl
} // namespace ig

#endif // IG_ENVI_WIDGET_IMPL_H
