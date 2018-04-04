/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/core/log/mgr.h"
#include "imagine/core/log/rec.h"
#include "imagine/core/log/sink.h"

#include <chrono>
#include <iostream>
#include <iomanip>

namespace ig {

void log_mgr::flush()
{ for (auto& sink : sinks_) sink->flush(); }

void log_mgr::push_rec(const log_rec& rec)
{ for (auto& sink : sinks_) sink->consume(rec); }

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
  log_rec r{
    type,
    "", // src.function_name,
    "", // src.file_name,
    0,  // src.line,
    buffer_.str()};
  buffer_.clear(); buffer_.str(std::string{});
}

log_mgr& log_mgr::get() {
  static log_mgr l;
  return l;
}

log_mgr::formatter log_mgr::default_format = [](const log_rec& r) {
  std::stringstream ss{};
  // system-wide real-time wall clock
  // maps to c-style time
  auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ss << std::put_time(std::localtime(&tt), "%c");

  switch (r.type) {
    case log_t::dbg:   ss << " - DEBUG [" << r.func << '@' << r.line << "] "; break;
    case log_t::info:  ss << " - INFO  "; break;
    case log_t::warn:  ss << " - WARN  "; break;
    case log_t::err:   ss << " - ERR   "; break; }

  ss << r.message;
  return ss.str();
};
log_mgr::sink_ptr log_mgr::default_sink = std::make_shared<log_sink>(std::cout);

} // namespace ig
