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

#include "imagine/envi/impl_arch/lib_impl.h"
#include "imagine/envi/lib.h"

namespace ig   {
namespace impl {

lib_native::lib_native()
  : path_{}
  , handle_{nullptr} {}

lib_native::lib_native(const std::string& path)
  : path_{path}
  , handle_{nullptr} {}

} // namespace impl

auto lib::resolve(const char* symbol) -> funcptr_type {
  if (native_->handle_) {
    auto proc = GetProcAddress(native_->handle_, symbol);
    return reinterpret_cast<funcptr_type>(proc);
  } else {
    return nullptr;
  }
}

bool lib::open(const std::string& path) {
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

void lib::close() {
  if (native_->handle_) {
    FreeLibrary(native_->handle_);
    native_->handle_ = nullptr;
  }
}

} // namespace ig
