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

#ifndef IG_CORE_LOG_H
#define IG_CORE_LOG_H

#include "imagine/ig.h"

#include <sstream>
#include <functional>
#include <mutex>
#include <memory>
#include <vector>

namespace ig {

enum log_t { info, dbg, warn, fatal };

class log_context;
class log_sink;
class IG_API log {
public:
  using formatter_t = std::function<std::string (const log_context&)>;
  friend log_context;
  
  static void add_sink(const std::shared_ptr<log_sink>& sink);
  static void remove_sink(const std::shared_ptr<log_sink>& sink);

  log(const log&) = delete;
  log& operator=(const log&) = delete;

  static formatter_t default_format;
  static std::shared_ptr<log_sink> default_sink;

protected:
  constexpr log() = default;

  void push(const log_context& c);
  static log& get();

  static std::mutex mutex_;
  static std::vector< std::shared_ptr<log_sink> > sinks_;
};

class IG_API log_context {
public:
  constexpr log_context(log_t type, const char* func, const char* file, int32_t line);
  ~log_context();

  log_t type_;
  std::stringstream stream_;
  const char* func_, *file_; int32_t line_;
};

class log_sink {
public:
  friend log;
  log_sink(std::ostream& s, log::formatter_t format = log::default_format)
    : stream_{s}, formatter_{format} {}

private:
  void consume(const log_context& c) { stream_ << formatter_(c); }

  std::ostream& stream_;
  log::formatter_t formatter_;
};

} // namespace ig

#define IG_LOG(log_t) ig::log_context(log_t, IG_FUNC, __FILE__, __LINE__).stream_

#endif // IG_CORE_LOG_H
