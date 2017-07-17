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

#ifndef IG_CORE_LOG_H
#define IG_CORE_LOG_H

#include "imagine/ig.h"

#include <sstream>
#include <functional>
#include <mutex>
#include <vector>

namespace ig {

enum log_t { dbg, info, warn, err };

class log_context;
class log_sink;
class ig_api log {
public:
  using frm_type = std::function< std::string(const log_context&) >;
  using sink_type = std::shared_ptr<log_sink>;
  friend log_context;
  
  log& add_sink(const sink_type& sink);
  void remove_sink(const sink_type& sink);

  log(const log&) = delete;
  log& operator=(const log&) = delete;

  static log& get();
  static frm_type default_format;
  static sink_type default_sink;

private:
  log() = default;
  void push(const log_context& ctx);

private:
  std::mutex mutex_;
  std::vector<sink_type> sinks_{default_sink};
};

class log_context {
public:
  explicit log_context(log_t type, const char* func, const char* file, int32_t line)
    : type{type}
    , func{func}
    , file{file}
    , line{line} {}
  ~log_context() { log::get().push(*this); }

  log_t type;
  const char* func, * file; int32_t line; std::stringstream stream;
};

class log_sink {
public:
  explicit log_sink(std::ostream& stream, const log::frm_type& format = log::default_format)
    : stream_{stream}
    , format_{format} {}
  virtual ~log_sink() = default;

  virtual void consume(const log_context& ctx) { stream_ << format_(ctx); }

protected:
  std::ostream& stream_;
  log::frm_type format_;
};

} // namespace ig

using ig::log_t;
#define LOG(type) ig::log_context(type, __func__, __FILE__, __LINE__).stream

#endif // IG_CORE_LOG_H
