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

#ifndef INTERACT_DISPATCHER_H
#define INTERACT_DISPATCHER_H

#include "imagine.h"
#include <functional>
#include <memory>

namespace ig
{

namespace impl { struct dispatcher_native; }

class IG_API dispatcher
{
public:
  dispatcher();
  virtual ~dispatcher();

  virtual int run();
  virtual void exit(int return_code);

  virtual bool process_events();
  virtual void handle() const;
  virtual void tick(std::function<void ()> func);

  static dispatcher* get() { return self_; };

  dispatcher(const dispatcher&) = delete;
  dispatcher& operator=(const dispatcher&) = delete;

private:
  std::unique_ptr<impl::dispatcher_native> native_;
  std::function<void ()> tick_ = [] {};

  static dispatcher* self_;
};

} // namespace ig

#endif // INTERACT_DISPATCHER_H
