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

#include "imagine/core/log.h"

#include <iostream>
#include <algorithm>

namespace ig {

// log
log& log::get() {
  static log l; return l;
}

void log::add_sink(const std::shared_ptr<log_sink>& sink) {
  sinks_.push_back(sink); 
}

void log::remove_sink(const std::shared_ptr<log_sink>& sink) {
  sinks_.erase(std::remove(sinks_.begin(), sinks_.end(), sink), 
               sinks_.end()); 
}

void log::push(const log_context& c) {
  std::lock_guard<decltype(mutex_)> lock{mutex_};
  for (auto&& sink : sinks_) {
    sink->consume(c);
  }
}

log::formatter_t log::default_format = [](const log_context& c) {
  auto ss = std::stringstream{};
  switch (c.type_) {
  case log_t::info:  ss << "[Info]  "; break;
  case log_t::dbg:   ss << "[Debug] "; break;
  case log_t::warn:  ss << "[Warn]  "; break;
  case log_t::fatal: ss << "[Fatal] "; break;
  }

  ss << '[' << c.func_ << '@' << c.line_ << "] " << c.stream_.str() << std::endl;
  return ss.str();
};

// log_context
constexpr log_context::log_context(log_t type, const char* func, const char* file, int32_t line)
  : type_{type}, func_{func}, file_{file}, line_{line} {
}

log_context::~log_context() {
  log::get().push(*this);
}

decltype(log::default_sink) log::default_sink = std::make_shared<log_sink>(std::cout);
decltype(log::sinks_)       log::sinks_       = {log::default_sink};
decltype(log::mutex_)       log::mutex_;

} // namespace ig
