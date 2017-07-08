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

#ifndef IG_CORE_CALL_H
#define IG_CORE_CALL_H

#include "imagine/ig.h"

#include <functional>
#include <vector>

namespace ig {

template <typename Signature> class call;
template <typename R, typename... Args>
class call<R(Args...)> {
public:
  using sig_type  = R(Args...);
  using func_type = std::function< sig_type >;

  call() = default;

  auto connect(const func_type& fn);
  void emit(Args&&... args) const;
  void operator()(Args&&... args) const;

  template <typename collect> auto collect(Args&&... args) const;

  call(const call&) = delete;
  call& operator=(const call&) = delete;

private:
  template <typename Signature>
  class subscriber {
  public:
    friend call;
    explicit subscriber(call& call, const func_type& fn)
      : call_{call}
      , fn_{fn} {}
    void disconnect();

  private:
    call& call_;
    func_type fn_;
  }; using subscriber_type = subscriber<sig_type>;

  std::vector< std::shared_ptr<subscriber_type> > subs_;
};

// call
template <typename R, typename... Args>
auto call<R(Args...)>::connect(const func_type& fn) {
  subs_.emplace_back(std::make_shared<subscriber_type>(*this, fn));
  return subs_.back();
}

template <typename R, typename... Args>
void call<R(Args...)>::emit(Args&&... args) const {
  for (auto& sub : subs_)
    sub->fn_(std::forward<Args>(args)...);
}

template <typename R, typename... Args>
void call<R(Args...)>::operator()(Args&&... args) const {
  emit(std::forward<Args>(args)...);
}

template <typename R, typename... Args>
template <typename collect>
auto call<R(Args...)>::collect(Args&&... args) const {
  static_assert(std::is_default_constructible<collect>::value, "Collecter must be default constructible");
  static_assert(std::is_same<collect::value_type, R>::value,   "Collecter value type must be equal to call return type");

  collect collecter{};
  for (auto& sub : subs_)
    collecter.emplace_back(sub->fn_(std::forward<Args>(args)...));
  return collecter;
}

// call::subscriber
template <typename R, typename... Args>
template <typename Signature>
void call<R(Args...)>::subscriber<Signature>::disconnect() {
  call_.subs_.erase(std::remove_if(call_.subs_.begin(), call_.subs_.end(), [this](auto& sub) {
    return sub.get() == this;
  }), call_.subs_.end());
}

} // namespace ig

#endif // IG_CORE_CALL_H
