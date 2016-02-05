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

#ifndef CORE_SIGNAL_H
#define CORE_SIGNAL_H

#include "imagine.h"

#include <functional>
#include <memory>
#include <vector>

namespace ig
{

template <typename... TArgs>
class signal
{
public:
  using func_t = std::function<void (TArgs...)>;
  struct slot;

  constexpr signal() = default;

  auto connect(func_t&& func);
  void disconnect(std::shared_ptr<slot> slot);

  void emit(TArgs&&... args) const;
  void operator()(TArgs&&... args) const;

  signal(const signal&) = delete;
  signal& operator=(const signal&) = delete;

private:
  struct slot
  {
    slot(signal& sig) 
      : sig{sig} {}

    signal& sig;
    func_t func;
  };

  std::vector< std::shared_ptr<slot> > slots_;
};

template <typename... TArgs>
auto signal<TArgs...>::connect(func_t&& func)
{
  auto backslot = std::make_shared<slot>(*this);
  backslot->func = std::forward<func_t>(func);

  slots_.emplace_back(std::move(backslot));
  return slots_.back();
}

template <typename... TArgs>
void signal<TArgs...>::disconnect(std::shared_ptr<slot> slot)
{
  slots_.erase(std::find(slots_.begin(), slots_.end(), slot));
}

template <typename... TArgs>
void signal<TArgs...>::emit(TArgs&&... args) const
{
  for (auto slot : slots_)
    slot->func(std::forward<TArgs>(args)...);
}

template <typename... TArgs>
void signal<TArgs...>::operator()(TArgs&&... args) const
{
  emit(std::forward<TArgs>(args)...);
}

} // namespace ig

#endif // CORE_SIGNAL_H
