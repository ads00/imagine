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
  return false;
}

void library::close() {
}

auto library::resolve(const char* symbol) -> function_ptr {
  return nullptr;
}

} // namespace ig
