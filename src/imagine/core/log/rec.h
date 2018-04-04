/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_CORE_LOGREC_H
#define IG_CORE_LOGREC_H

#include "imagine/core/log/mgr.h"

namespace ig {

class log_rec {
public:
  explicit log_rec(
    log_t type,
    const char* func,
    const char* file,
    int32_t line,
    const std::string& message)
    : type{type}
    , func{func}
    , file{file}
    , line{line}
    , message{message} {}
  ~log_rec() { log_mgr::get().push_rec(*this); }

  log_t type;
  const char* func, * file; int32_t line; std::string message;
};

} // namespace ig

#endif // IG_CORE_LOGREC_H
