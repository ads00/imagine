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

#ifndef INTERACT_WINDOW_H
#define INTERACT_WINDOW_H

#include "imagine/interact/dispatcher.h"
#include "imagine/interact/events.h"
#include "imagine/core/flags.h"

namespace ig
{

namespace impl { struct window_native; }

class IG_API window
{
public:
  enum visibility_t { hidden, windowed, fullscreen };

  enum style_t
  {
    none      = 0x00,
    closable  = 0x01,
    resizable = 0x02,
    titlebar  = 0x04,
    default_  = closable | resizable | titlebar
  };

  friend class cursor;
  using style_ft = flags<style_t>;

  constexpr window();
  constexpr window(const std::string& caption, unsigned int width, unsigned int height, style_ft style = style_t::default_);
  virtual ~window();

  void close();
  void show();
  void hide();
  void focus();

  void move(int x, int y);
  void resize(unsigned int width, unsigned int height);

  bool opened() const;
  bool minimized() const;
  bool maximized() const;
  bool visible() const;

  events& events() const;
  cursor& cursor() const;

  int x() const;
  int y() const;
  unsigned int width() const;
  unsigned int height() const;
  const std::string& caption() const;

  void set_parent(const window* parent);

  void set_fullscreen(bool fullscreen);
  void set_caption(const std::string& caption);

  window(const window&) = delete;
  window& operator=(const window&) = delete;

private:
  std::unique_ptr<impl::window_native> native_;
};

} // namespace ig

#endif // INTERACT_WINDOW_H
