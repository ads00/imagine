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

#include "imagine/envi/impl_arch/widget_impl.h"
#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/widget.h"

namespace ig {

widget::widget()
  : native_{std::make_unique<impl::widget_impl>(*this)}
  , cursor_{cursor_shape::none} {}

widget::widget(const dispatch& dsp, widget_styles styles, const std::string& caption, uint32_t w, uint32_t h)
  : native_{std::make_unique<impl::widget_impl>(*this, styles, caption, w, h)}
  , cursor_{cursor_shape::arrow} { 

  link_ = [this, &widgets = dsp.native_->widgets_]() {
    auto it = 
      std::remove_if(
        widgets.begin(),
        widgets.end(),
        [this](auto& w) { return w == native_.get(); });
    if (it != widgets.end()) {
      widgets.erase(it, widgets.end());
    } else {
      widgets.emplace_back(native_.get());
    }
  }; link_();
}

widget::~widget() {
  close();
  if (link_) link_();
}

uint32_t widget::get_width() const
{ return native_->w_; }

uint32_t widget::get_height() const
{ return native_->h_; }

int32_t widget::get_x() const
{ return native_->x_; }

int32_t widget::get_y() const
{ return native_->y_; }

widget_visibility widget::get_visibility() const
{ return native_->visibility_; }

const std::string& widget::get_caption() const
{ return native_->caption_; }

// Native implementations
//

// void widget::close();
// void widget::show();
// void widget::hide();
// void widget::focus();
// void widget::resize(uint32_t width, uint32_t height);
// void widget::move(int32_t x, int32_t y);
// bool widget::opened() const;
// bool widget::minimized() const;
// bool widget::maximized() const;
// bool widget::visible() const;
// auto widget::get_handle() const -> handle*;
// void widget::set_fullscreen(bool fullscreen);
// void widget::set_caption(const std::string& title);
// void widget::set_parent(const widget* parent);

} // namespace ig
