/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_CURSOR_H
#define IG_ENVI_CURSOR_H

#include "imagine/ig.h"

namespace ig   {
namespace impl { class cursor_impl; class widget_impl; }

enum class cursor_shape;

class widget;
class IG_API cursor {
public:
  friend impl::widget_impl;

  explicit cursor(cursor_shape shape);
  virtual ~cursor();

  void refresh() const;
  void reshape(cursor_shape shape);

  static void clip(const widget& widget);
  static void move(int32_t x, int32_t y, const widget* widget = nullptr);

private:
  std::unique_ptr<impl::cursor_impl> native_;
};

enum class cursor_shape {
  none,
  arrow,
  cross,
  hand,
  pointer,
  help,
  progress,
  wait,
  text,
  sizeall,
  resize_n, resize_s, resize_e, resize_w, resize_nw, resize_se, resize_ne, resize_sw,
  bitmap };

} // namespace ig

#endif // IG_ENVI_CURSOR_H
