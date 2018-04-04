/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/impl_arch/widget_impl.h"
#include "imagine/envi/dispatch.h"

namespace ig   {
namespace impl {

dispatch_impl::dispatch_impl()
  : return_code_{-1}
  , running_{false} {}

} // namespace impl

void dispatch::process_events() {
}

} // namespace ig
