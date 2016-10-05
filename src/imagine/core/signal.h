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

template <typename Signature> class signal;
template <typename R, typename... Args>
class signal<R(Args...)> {
public:
  using sig_type  = R(Args...);
  using func_type = std::function<sig_type>;

  signal() = default;

  auto connect(const func_type& fn);
  void emit(Args&&... args) const;
  void operator()(Args&&... args) const;

  template <typename Collect> auto collect(Args&&... args) const;

  signal(const signal&) = delete;
  signal& operator=(const signal&) = delete;

private:
  template <typename Signature>
  class slot {
  public:
    friend signal;
    explicit slot(signal& signal, const func_type& fn)
      : signal_{signal}
      , fn_{fn} {}
    void disconnect();

  private:
    signal& signal_;
    func_type fn_;
  }; using slot_type = slot<sig_type>;

  std::vector< std::shared_ptr<slot_type> > slots_;
};

// signal
template <typename R, typename... Args>
auto signal<R(Args...)>::connect(const func_type& fn) {
  slots_.emplace_back(std::make_shared<slot_type>(*this, fn));
  return slots_.back();
}

template <typename R, typename... Args>
void signal<R(Args...)>::emit(Args&&... args) const {
  for (auto& slot : slots_)
    slot->fn_(std::forward<Args>(args)...);
}

template <typename R, typename... Args>
void signal<R(Args...)>::operator()(Args&&... args) const {
  emit(std::forward<Args>(args)...);
}

template <typename R, typename... Args>
template <typename Collect>
auto signal<R(Args...)>::collect(Args&&... args) const {
  static_assert(std::is_default_constructible<Collect>::value, "Collecter must be default constructible");
  static_assert(std::is_same<Collect::value_type, R>::value,   "Collecter value type must be equal to signal return type");

  Collect collecter{};
  for (auto& slot : slots_)
    collecter.emplace_back(slot->fn_(std::forward<Args>(args)...));
  return collecter;
}

// signal::slot
template <typename R, typename... Args>
template <typename Signature>
void signal<R(Args...)>::slot<Signature>::disconnect() {
  signal_.slots_.erase(std::remove_if(signal_.slots_.begin(), signal_.slots_.end(), [this](auto& slot) {
    return slot.get() == this;
  }), signal_.slots_.end());
}

} // namespace ig

#endif // IG_CORE_SIGNAL_H
