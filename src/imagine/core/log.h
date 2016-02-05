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

#ifndef CORE_LOG_H
#define CORE_LOG_H

#include "imagine.h"

#include <sstream>
#include <mutex>
#include <vector>
#include <functional>

namespace ig
{

enum log_t { info, dbg, warn, fatal };

struct IG_API log_context
{
  constexpr log_context(log_t type, const char* func, const char* file, int line);
  ~log_context();

  log_t type_;
  std::ostringstream stream_;
  const char* func_, * file_; int line_;
};

class log_sink;
class IG_API log
{
public:
  friend log_context;
  using formatter_t = std::function<std::string (const log_context&)>;

  static void add_sink(std::shared_ptr<log_sink> sink);
  static void remove_sink(std::shared_ptr<log_sink> sink);

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

class log_sink
{
public:
  log_sink(std::ostream& s, log::formatter_t format = log::default_format)
    : stream_{s}, formatter_{format} {}

  void consume(const log_context& c) { stream_ << formatter_(c); }

private:
  std::ostream& stream_;
  log::formatter_t formatter_;
};

} // namespace ig

#define IG_LOG(log_t) ig::log_context(log_t, IG_FUNC, __FILE__, __LINE__).stream_

#endif // CORE_LOG_H
