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

namespace ig
{

constexpr library::library()
  : native_{std::make_unique<impl::library_native>()}
{
}

library::library(const std::string& path)
  : native_{std::make_unique<impl::library_native>(path)}
{
  open(path);
}

library::~library()
{
  close();
}

bool library::loaded() const
{
  return native_->handle_ != nullptr;
}

auto library::resolve(const std::string& path, const char* symbol) -> func_ptr
{
  library l{path};
  return l.resolve(symbol);
}

// Native implementations
//

// auto library::resolve(const char* symbol) -> func_ptr;
// bool library::open(const std::string& path);
// void library::close();

} // namespace ig