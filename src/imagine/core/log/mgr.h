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

#ifndef IG_CORE_LOGMGR_H
#define IG_CORE_LOGMGR_H

#include "imagine/ig.h"

#include <vector>
#include <sstream>

namespace ig {

enum log_t { dbg, info, warn, err };

class log_sink;
class log_entry;
class IG_API log_mgr {
public:
  using formatter = std::function< std::string(const log_entry&) >;
  using sink_ptr  = std::shared_ptr<log_sink>;
  
  void flush();
  void push_entry(const log_entry& entry);

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
