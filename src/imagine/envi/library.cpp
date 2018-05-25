/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/library_impl.h"
#include "imagine/envi/library.h"

namespace ig {

library::library()
  : native_{std::make_unique<impl::library_impl>()} {}

library::library(const std::string& path)
  : native_{std::make_unique<impl::library_impl>(path)} { open(path); }

library::~library() {
  close();
}

bool library::is_open() const
{ return native_->handle_ != nullptr; }

// Native implementations
//

// bool library::open(const std::string& path);
// void library::close();
// auto library::resolve(const char* symbol) -> function_ptr;

} // namespace ig
