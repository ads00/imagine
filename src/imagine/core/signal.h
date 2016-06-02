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

template <typename Signature> class signal_slot;
template <typename Signature> class signal;

template <typename R, typename... Args>
class signal<R (Args...)> {
public:
  using fn_t   = std::function<R (Args...)>;
  using slot_t = signal_slot  <R (Args...)>;
  friend slot_t;

  constexpr signal() = default;

  auto connect(fn_t&& fn);
  void disconnect(const std::shared_ptr<slot_t>& slot);

  template <typename Collect>
  auto collect(Args&&... args) const;

  void emit(Args&&... args) const;
  void operator()(Args&&... args) const;

  signal(const signal&) = delete;
  signal& operator=(const signal&) = delete;

private:
  struct ctor{};
  std::vector< std::shared_ptr<slot_t> > slots_;
};

template <typename Signature>
class signal_slot {
public:
  using sig_t = signal<Signature>;
  using fn_t  = typename sig_t::fn_t;

  constexpr signal_slot(typename sig_t::ctor, sig_t& signal, fn_t&& fn) 
    : signal_{signal}, fn_{fn} {}

  sig_t& signal_; 
  fn_t fn_;
};

template <typename R, typename... Args>
auto signal<R (Args...)>::connect(fn_t&& fn) {
  slots_.emplace_back(std::make_shared<slot_t>(ctor{}, *this, std::forward<fn_t>(fn)));
  return slots_.back();
}

template <typename R, typename... Args>
void signal<R (Args...)>::disconnect(const std::shared_ptr<slot_t>& slot) {
  slots_.erase(std::remove(slots_.begin(), slots_.end(), slot), 
               slots_.end());
}

template <typename R, typename... Args>
template <typename Collect>
auto signal<R (Args...)>::collect(Args&&... args) const {
  auto collecter = Collect{};
  for (auto&& slot : slots_) {
    collecter.emplace_back(slot->fn_(std::forward<Args>(args)...));
  } return collecter;
}

template <typename R, typename... Args>
void signal<R (Args...)>::emit(Args&&... args) const {
  for (auto&& slot : slots_) {
    slot->fn_(std::forward<Args>(args)...);
  }
}

template <typename R, typename... Args>
void signal<R (Args...)>::operator()(Args&&... args) const {
  emit(std::forward<Args>(args)...);
}

} // namespace ig

#endif // IG_CORE_SIGNAL_H
