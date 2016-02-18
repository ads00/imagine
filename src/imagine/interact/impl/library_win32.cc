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

#include "imagine/interact/impl/library_native.h"
#include "imagine/interact/library.h"

namespace ig   {
namespace impl {

library_native::library_native()
  : path_{}, handle_{nullptr}
{
}

library_native::library_native(const std::string& path)
  : path_{path}, handle_{nullptr}
{
}

} // namespace impl

auto library::resolve(const char* symbol) -> func_ptr
{
  if (!native_->handle_) return nullptr;
  else
  {
    func_ptr ptr = reinterpret_cast<func_ptr>(
      GetProcAddress(native_->handle_, symbol));

    if (!ptr)
    {
      throw std::runtime_error
        ("Failed to resolve symbol from " + native_->path_);
    }

    return ptr;
  }
}

bool library::open(const std::string& path)
{
  if (native_->handle_)
  {
    if (native_->path_ != path) close();
    else return true;
  }

  native_->handle_ = LoadLibraryA(path.c_str());
  if (!native_->handle_)
  {
    throw std::runtime_error
      ("Failed to open dynamic library");
  }

  native_->path_ = path;
  return true;
}

void library::close()
{
  if (!native_->handle_) return;
  else
  {
    FreeLibrary(native_->handle_);
    native_->handle_ = nullptr;
  }
}

} // namespace ig
