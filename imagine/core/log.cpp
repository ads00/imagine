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

#include "imagine/core/log.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

namespace ig {

// log
log& log::get() {
  static log l; 
  return l;
}

log& log::add_sink(const std::shared_ptr<log_sink>& sink) {
  sinks_.push_back(sink); 
  return *this;
}

void log::remove_sink(const std::shared_ptr<log_sink>& sink) {
  sinks_.erase(std::remove(sinks_.begin(), sinks_.end(), sink), 
               sinks_.end()); 
}

void log::push(const log_context& ctx) {
  std::lock_guard<decltype(mutex_)> lock{mutex_};
  for (auto& sink : sinks_)
    sink->consume(ctx);
}

log::formatter_type log::default_format = [](const log_context& c) {
  auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::stringstream 
    ss{};
  ss << std::put_time(std::localtime(&tt), "%c");

  switch (c.type) {
  case log_t::dbg:   ss << " - DEBUG [" << c.func << '@' << c.line << "] ";
    break;
  case log_t::info:  ss << " - INFO  "; break;
  case log_t::warn:  ss << " - WARN  "; break;
  case log_t::err:   ss << " - ERR   "; break;
  }
  ss << c.stream.str() << std::endl;
  return ss.str();
};

// log_context
log_context::log_context(log_t type, const char* func, const char* file, int32_t line)
  : type{type}
  , func{func}
  , file{file}
  , line{line} {}

log_context::~log_context() {
  log::get().push(*this);
}

std::shared_ptr<log_sink> log::default_sink = std::make_shared<log_sink>(std::cout);

} // namespace ig
