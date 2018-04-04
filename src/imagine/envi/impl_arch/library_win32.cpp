/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/library_impl.h"
#include "imagine/envi/library.h"

namespace ig   {
namespace impl {

library_impl::library_impl()
  : path_{}
  , handle_{nullptr} {}

library_impl::library_impl(const std::string& path)
  : path_{path}
  , handle_{nullptr} {}

} // namespace impl

bool library::open(const std::string& path) {
  if (native_->handle_) {
    if (native_->path_ != path) {
      close();
    } else {
      return true;
    }
  }

  native_->handle_ = LoadLibraryA(path.c_str());
  native_->path_ = path;
  return native_->handle_ != nullptr;
}

void library::close() {
  if (native_->handle_) {
    FreeLibrary(native_->handle_);
    native_->handle_ = nullptr;
  }
}

auto library::resolve(const char* symbol) -> function_ptr {
  if (native_->handle_) {
    auto proc = GetProcAddress(native_->handle_, symbol);
    return reinterpret_cast<function_ptr>(proc);
  } else {
    return nullptr;
  }
}

} // namespace ig
