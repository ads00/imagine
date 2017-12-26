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

#include "imagine/core/log/mgr.h"
#include "imagine/core/log/entry.h"
#include "imagine/core/log/sink.h"

#include <chrono>
#include <iostream>
#include <iomanip>

namespace ig {

log_mgr& log_mgr::get() {
  static log_mgr l;
  return l;
}

void log_mgr::flush()
{ for (auto& sink : sinks_) sink->flush(); }

void log_mgr::push_entry(const log_entry& entry)
{ for (auto& sink : sinks_) sink->consume(entry); }

void log_mgr::clear()
{ sinks_.clear(); }

void log_mgr::add_sink(const sink_ptr& sink) {
  assert(sink != nullptr);
  sinks_.emplace_back(sink);
}

void log_mgr::remove_sink(const sink_ptr& sink) {
  sinks_.erase(
    std::remove(sinks_.begin(), sinks_.end(), sink),
    sinks_.end());
}

void log_mgr::write(log_t type, const char* format) {
  buffer_ << format;
  log_entry e{
    type,
    "", // src.function_name,
    "", // src.file_name,
    0,  // src.line,
    buffer_.str()};
  buffer_.clear(); buffer_.str(std::string{});
}

log_mgr::formatter log_mgr::default_format = [](const log_entry& e) {
  std::stringstream ss{};
  // system-wide real-time wall clock
  // maps to c-style time
  auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&tt), "%c");

  switch (e.type) {
    case log_t::dbg:   ss << " - DEBUG [" << e.func << '@' << e.line << "] "; break;
    case log_t::info:  ss << " - INFO  "; break;
    case log_t::warn:  ss << " - WARN  "; break;
    case log_t::err:   ss << " - ERR   "; break; }

  ss << e.message;
  return ss.str();
};
log_mgr::sink_ptr log_mgr::default_sink = std::make_shared<log_sink>(std::cout);

} // namespace ig
