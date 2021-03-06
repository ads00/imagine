/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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
