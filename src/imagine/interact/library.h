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

#ifndef INTERACT_LIBRARY_H
#define INTERACT_LIBRARY_H

#include "imagine.h"
#include <memory>

namespace ig
{

namespace impl { struct library_native; }

class IG_API library
{
public:
  typedef void (*func_ptr)();

  constexpr library();
  library(const std::string& path);
  ~library();

  auto resolve(const char* symbol) -> func_ptr;
  
  bool open(const std::string& path);
  void close();

  bool loaded() const;

  static auto resolve(const std::string& path, const char* symbol) -> func_ptr;

  library(const library&) = delete;
  library& operator=(const library&) = delete;

private:
  std::unique_ptr<impl::library_native> native_;
};

} // namespace ig

#endif // INTERACT_LIBRARY_H
