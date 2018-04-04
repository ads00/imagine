/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_DISPATCH_H
#define IG_ENVI_DISPATCH_H

#include "imagine/ig.h"

namespace ig   {
namespace impl { class dispatch_impl; }

class IG_API dispatch {
public:
  friend class widget;

  dispatch();
  virtual ~dispatch();

  virtual int32_t run();
  virtual void exit(int32_t return_code);

  virtual void process_events();
  template <typename Callable> void tick(Callable&& fn) { tick_ = fn; }

  dispatch(const dispatch&) = delete;
  dispatch& operator=(const dispatch&) = delete;

private:
  std::unique_ptr<impl::dispatch_impl> native_;
  task tick_ = [] {};
};

} // namespace ig

#endif // IG_ENVI_DISPATCH_H
