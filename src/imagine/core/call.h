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

#include <vector>

namespace ig {

template <typename Signature>
class call;
template
< typename ReturnType,
  typename... Args >
class call<ReturnType(Args...)> {
public:
  using signature = ReturnType(Args...);
  using callback  = std::function< signature >;

  call() = default;

  void emit(Args&&... args) const;
  template <typename Collect> void emit(Args&&... args, Collect&& fn) const;

  void clear();
  auto connect(const callback& fn);

  call(const call&) = delete;
  call& operator=(const call&) = delete;

private:
  class subscriber {
  public:
    friend call;
    explicit subscriber(call& call, const callback& fn)
      : call_{call}
      , fn_{fn} {}
    void disconnect();

  private:
    call& call_;
    callback fn_;
  }; using subscriber_ptr = std::shared_ptr<subscriber>;

  std::vector<subscriber_ptr> subs_;
};

// call
template
< typename ReturnType,
  typename... Args >
void call<ReturnType(Args...)>::emit(Args&&... args) const
{ for (auto& sub : subs_) sub->fn_(std::forward<Args>(args)...); }

template
< typename ReturnType,
  typename... Args >
template <typename Collect>
void call<ReturnType(Args...)>::emit(Args&&... args, Collect&& fn) const
{ for (auto& sub : subs_) fn(sub->fn_(std::forward<Args>(args)...)); }

template
< typename ReturnType,
  typename... Args >
void call<ReturnType(Args...)>::clear()
{ subs_.clear(); }

template
< typename ReturnType,
  typename... Args >
auto call<ReturnType(Args...)>::connect(const callback& fn) {
  assert(fn != nullptr);
  return subs_.emplace_back(std::make_shared<subscriber>(*this, fn));
}

// call::subscriber
template
< typename ReturnType,
  typename... Args >
void call<ReturnType(Args...)>::subscriber::disconnect() {
  call_.subs_.erase(
    std::remove_if(
      call_.subs_.begin(),
      call_.subs_.end(),
      [this](auto& sub) { return sub.get() == this; }),
    call_.subs_.end());
}

} // namespace ig

#endif // IG_CORE_CALL_H
