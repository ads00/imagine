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

#ifndef IG_ENVI_WINDOW_H
#define IG_ENVI_WINDOW_H

#include "imagine/envi/events.h"
#include "imagine/envi/cursor.h"
#include "imagine/core/flags.h"

namespace ig   {
namespace impl { class window_native; }

struct window_handle{};
class IG_API window : public events {
public:
  enum class visibility_t { hidden, windowed, fullscreen };
  enum class style_t : uint32_t {
    none        = 0x00,
    closable    = 0x01,
    resizable   = 0x02,
    titlebar    = 0x04,
    interactive = closable | resizable | titlebar
  }; using style_flags = flags<style_t>;

  friend cursor;

  window();
  window(const std::string& caption, uint32_t width, uint32_t height, style_flags style);
  virtual ~window();

  void close();
  void show();
  void hide();
  void focus();

  void move(int32_t x, int32_t y);
  void resize(uint32_t width, uint32_t height);

  bool opened() const;
  bool minimized() const;
  bool maximized() const;
  bool visible() const;

  int32_t x() const;
  int32_t y() const;
  uint32_t width() const;
  uint32_t height() const;

  auto handle() const -> window_handle*;
  auto visibility() const -> visibility_t;

  const std::string& caption() const;

  void set_fullscreen(bool fullscreen);
  void set_caption(const std::string& caption);
  void set_parent(const window* parent);

  window(const window&) = delete;
  window& operator=(const window&) = delete;

  cursor cursor_;

protected:
  std::unique_ptr<impl::window_native> native_;
};

} // namespace ig

#endif // IG_ENVI_WINDOW_H
