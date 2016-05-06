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

#ifndef IG_CORE_SIGNAL_H
#define IG_CORE_SIGNAL_H

#include "imagine/ig.h"

#include <functional>
#include <memory>
#include <vector>

namespace ig {

template <typename... Args> class signal_slot;
template <typename... Args>
class signal {
public:
  using fn_t   = std::function<void (Args...)>;
  using slot_t = signal_slot<Args...>;
  friend slot_t;

  constexpr signal() = default;

  auto connect(fn_t&& fn);
  void disconnect(const std::shared_ptr<slot_t>& slot);

  void emit(Args&&... args) const;
  void operator()(Args&&... args) const;

  signal(const signal&) = delete;
  signal& operator=(const signal&) = delete;

private:
  struct ctor{};
  std::vector< std::shared_ptr<slot_t> > slots_;
};

template <typename... Args>
class signal_slot {
public:
  using sig_t = signal<Args...>;
  using fn_t  = typename sig_t::fn_t;

  constexpr signal_slot(typename sig_t::ctor, sig_t& signal, fn_t&& fn) : signal_{signal}, fn_{fn} {}
  sig_t& signal_; fn_t fn_;
};

template <typename... Args>
auto signal<Args...>::connect(fn_t&& fn) {
  slots_.emplace_back(std::make_shared<slot_t>(ctor{}, *this, std::forward<fn_t>(fn)));
  return slots_.back();
}

template <typename... Args>
void signal<Args...>::disconnect(const std::shared_ptr<slot_t>& slot) {
  slots_.erase(std::remove(slots_.begin(), slots_.end(), slot), 
               slots_.end());
}

template <typename... Args>
void signal<Args...>::emit(Args&&... args) const {
  for (auto&& slot : slots_) {
    slot->fn_(std::forward<Args>(args)...);
  }
}

template <typename... Args>
void signal<Args...>::operator()(Args&&... args) const {
  emit(std::forward<Args>(args)...);
}

} // namespace ig

#endif // IG_CORE_SIGNAL_H
