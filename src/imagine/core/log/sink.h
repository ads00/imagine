/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_CORE_LOGSINK_H
#define IG_CORE_LOGSINK_H

#include "imagine/core/log/mgr.h"

#include <ostream>

namespace ig {

class log_sink {
public:
  explicit log_sink(
    std::ostream& stream,
    const log_mgr::formatter& format = log_mgr::default_format)
    : stream_{stream}
    , format_{format} {}
  virtual ~log_sink() = default;

  virtual void flush()
  { stream_.flush(); }
  virtual void consume(const log_rec& rec)
  { stream_ << format_(rec); }

protected:
  std::ostream& stream_;
  log_mgr::formatter format_;
};

} // namespace ig

#endif // IG_CORE_LOGSINK_H
