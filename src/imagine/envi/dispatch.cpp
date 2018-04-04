/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/dispatch.h"

namespace ig {

dispatch::dispatch()
  : native_{std::make_unique<impl::dispatch_impl>()} {}

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

// Native implementations
//

// void dispatch::process_events();

} // namespace ig
