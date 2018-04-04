/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_CORE_LOGMGR_H
#define IG_CORE_LOGMGR_H

#include "imagine/ig.h"

#include <vector>
#include <sstream>

namespace ig {

enum log_t { dbg, info, warn, err };

class log_rec;
class log_sink;
class IG_API log_mgr {
public:
  using formatter = std::function< std::string(const log_rec&) >;
  using sink_ptr  = std::shared_ptr<log_sink>;

  void flush();
  void push_rec(const log_rec& rec);

  void clear();
  void add_sink(const sink_ptr& sink);
  void remove_sink(const sink_ptr& sink);

  template <typename T, typename... Args>
  void write(
    log_t type,
    const char* format,
    T&& arg,
    Args&&... args);
  void write(log_t type, const char* format);

  log_mgr(const log_mgr&) = delete;
  log_mgr& operator=(const log_mgr&) = delete;

  static log_mgr& get();
  static formatter default_format; static sink_ptr default_sink;

private:
  log_mgr() = default;

  std::stringstream buffer_;
  std::vector<sink_ptr> sinks_{default_sink};
};

template <typename T, typename... Args>
void log_mgr::write(log_t type, const char* format, T&& arg, Args&&... args) {
  auto* p = format;
  auto* s = p;
  while (*p) {
    char c = *p++;
    if (c != '{' && c != '}') continue;
    if (*p == c) {
      buffer_.write(s, p - s);
      s = ++p;
      continue;
    } else {
      buffer_.write(s, (p - 1) - s);
      buffer_ << arg; write(type, ++p, std::forward<Args>(args)...);
      return;
    }
  } write(type, s);
}

} // namespace ig

#endif // IG_CORE_LOGMGR_H
