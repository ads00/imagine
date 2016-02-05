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

#include "imagine/interact/window.h"
#include "imagine/interact/impl/window_native.h"

namespace ig
{

constexpr window::window()
  : native_{std::make_unique<impl::window_native>(this)}
{
}

constexpr window::window(const std::string& caption, unsigned int width, unsigned int height, style_ft style)
  : native_{std::make_unique<impl::window_native>(this, caption, width, height, style)}
{
}

window::~window()
{
  close();
}

cursor& window::cursor() const
{
  return native_->cursor_;
}

events& window::events() const
{
  return *(native_->events_);
}

int window::x() const
{
  return native_->x_;
}

int window::y() const
{
  return native_->y_;
}

unsigned int window::width() const
{
  return native_->width_;
}

unsigned int window::height() const
{
  return native_->height_;
}

const std::string& window::caption() const
{
  return native_->caption_;
}

// Native implementations
//

// void window::close();
// void window::show();
// void window::hide();
// void window::focus();
// void window::move(int x, int y);
// void window::resize(unsigned int width, unsigned int height);
// bool window::opened() const;
// bool window::minimized() const;
// bool window::maximized() const;
// bool window::visible() const;
// void window::set_parent(const Window* parent);
// void window::set_fullscreen(bool fullscreen);
// void window::set_caption(const std::string& title);

} // namespace ig
