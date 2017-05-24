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

#include "imagine/envi/impl_arch/window_impl.h"
#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/window.h"

#include <algorithm>

namespace ig {

window::window()
  : native_{std::make_unique<impl::window_native>(*this)}
  , cursor{cursor_shape::none} {}

window::window(const dispatch& dsp, types_t types, const std::string& caption, uint32_t w, uint32_t h)
  : native_{std::make_unique<impl::window_native>(*this, types, caption, w, h)}
  , cursor{cursor_shape::arrow} { 

  link_ = [this, &wins = dsp.native_->windows_]() {
    auto it = std::remove_if(wins.begin(), wins.end(), [this](auto& w) { return w == native_.get(); });
    it != wins.end()
      ? wins.erase(it, wins.end())
      : wins.emplace_back(native_.get());
  }; link_();
}

window::~window() {
  close();
  if (link_) link_();
}

uint32_t window::get_width() const {
  return native_->w_;
}

uint32_t window::get_height() const {
  return native_->h_;
}

int32_t window::get_x() const {
  return native_->x_;
}

int32_t window::get_y() const {
  return native_->y_;
}

window_visibility window::get_visibility() const {
  return native_->visibility_;
}

const std::string& window::get_caption() const {
  return native_->caption_;
}

// Native implementations
//

// void window::close();
// void window::show();
// void window::hide();
// void window::focus();
// void window::resize(uint32_t width, uint32_t height);
// void window::move(int32_t x, int32_t y);
// bool window::opened() const;
// bool window::minimized() const;
// bool window::maximized() const;
// bool window::visible() const;
// auto window::get_handle() const -> window_handle*;
// void window::set_fullscreen(bool fullscreen);
// void window::set_caption(const std::string& title);
// void window::set_parent(const window* parent);

} // namespace ig
