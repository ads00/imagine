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
  virtual void consume(const log_rec& rec) { stream_ << format_(rec); }

protected:
  std::ostream& stream_;
  log_mgr::formatter format_;
};

} // namespace ig

#endif // IG_CORE_LOGSINK_H
