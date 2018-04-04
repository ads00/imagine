/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_DISPATCH_IMPL_H
#define IG_ENVI_DISPATCH_IMPL_H

#include "imagine/envi/impl_arch/config_impl.h"

#include <atomic>
#include <vector>

namespace ig   {
namespace impl {

static std::string ig_widget_class = "ig_widgetclass";

class widget_impl;
class dispatch_impl {
public:
  dispatch_impl();
  ~dispatch_impl() = default;

  std::atomic_int return_code_;
  std::atomic_bool running_;
  std::vector<widget_impl*> widgets_;

  #if defined(IG_WIN)
  static HINSTANCE reg();
  static LRESULT CALLBACK proc(
    HWND hwnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam);
  #endif
};

} // namespace impl
} // namespace ig

#endif // IG_ENVI_DISPATCH_IMPL_H
