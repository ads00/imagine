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

#include "imagine/envi/arch/dispatch_impl.h"
#include "imagine/envi/dispatch.h"

namespace ig {

dispatch::dispatch()
  : native_{std::make_unique<impl::dispatch_native>()} {}

dispatch::~dispatch() = default;

int32_t dispatch::run() {
  assert(!native_->running_ && "Dispatcher already running");
  native_->running_ = true;

  while (native_->running_)
    process_events();
  return native_->return_code_;
}

void dispatch::exit(int32_t return_code) {
  native_->return_code_ = return_code;
  native_->running_     = false;
}

void dispatch::tick(const func_type& fn) {
  tick_ = fn;
}

// Native implementations
//

// void dispatch::process_events();

} // namespace ig
