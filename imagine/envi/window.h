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

#ifndef IG_ENVI_WINDOW_H
#define IG_ENVI_WINDOW_H

#include "imagine/envi/dispatch.h"
#include "imagine/envi/events.h"
#include "imagine/envi/cursor.h"
#include "imagine/core/flags.h"

namespace ig   {
namespace impl { class window_native; }

enum class window_visibility { hidden, windowed, fullscreen };
enum class window_gesture    { tap, pan, pinch, swipe };

struct window_handle {};
class ig_api window : public events {
public:
  enum type_t : uint32_t {
    none     = 0x000, titlebar  = 0x001,
    closable = 0x002, resizable = 0x004,
    interactive = closable | resizable | titlebar
  }; using types_t = flags<type_t>;

  window();
  explicit window(const dispatch& dsp, types_t types, const std::string& caption, uint32_t w, uint32_t h);
  virtual ~window();

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

  auto get_handle() const -> window_handle*;

  uint32_t get_width() const;
  uint32_t get_height() const;
  int32_t get_x() const;
  int32_t get_y() const;

  window_visibility get_visibility() const;
  const std::string& get_caption() const;

  void set_fullscreen(bool fullscreen);
  void set_caption(const std::string& caption);
  void set_parent(const window* parent);

  window(const window&) = delete;
  window& operator=(const window&) = delete;

  cursor cursor;

private:
  std::unique_ptr<impl::window_native> native_;
  std::function
  < void() > link_ = [] {};
};

} // namespace ig

#endif // IG_ENVI_WINDOW_H
