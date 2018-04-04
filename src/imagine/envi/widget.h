/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_WIDGET_H
#define IG_ENVI_WIDGET_H

#include "imagine/envi/dispatch.h"
#include "imagine/envi/cursor.h"
#include "imagine/envi/interface.h"
#include "imagine/core/flags.h"

namespace ig   {
namespace impl { class widget_impl; }

enum class widget_style : uint32_t; using widget_styles = flags<widget_style>;
enum class widget_visibility;

class IG_API widget : public interfaces {
public:
  widget();
  explicit widget(const dispatch& dsp, widget_styles styles, const std::string& caption, uint32_t w, uint32_t h);
  virtual ~widget();

  void close();
  void show();
  void hide();
  void focus();

  void resize(uint32_t width, uint32_t height);
  void move(int32_t x, int32_t y);

  bool opened() const;
  bool minimized() const;
  bool maximized() const;
  bool visible() const;

  struct handle {};
  auto get_handle() const -> handle*;

  uint32_t get_width() const;
  uint32_t get_height() const;
  int32_t get_x() const;
  int32_t get_y() const;

  widget_visibility get_visibility() const;
  const std::string& get_caption() const;

  void set_fullscreen(bool fullscreen);
  void set_caption(const std::string& caption);
  void set_parent(const widget* parent);

  widget(const widget&) = delete;
  widget& operator=(const widget&) = delete;

public:
  // icon   icon_;
  cursor cursor_;

private:
  std::unique_ptr<impl::widget_impl> native_;
  task link_ = [] {};
};

enum class widget_style : uint32_t {
  none     = 0x000, titlebar  = 0x001,
  closable = 0x002, resizable = 0x004, interactive = closable | resizable | titlebar };

enum class widget_visibility { hidden, windowed, fullscreen };

} // namespace ig

#endif // IG_ENVI_WIDGET_H
