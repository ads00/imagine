/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_LIBRARY_H
#define IG_ENVI_LIBRARY_H

#include "imagine/ig.h"

namespace ig   {
namespace impl { class library_impl; }

class IG_API library {
public:
  using function_ptr = void (*)();

  library();
  explicit library(const std::string& path);
  virtual ~library();

  bool open(const std::string& path);
  void close();
  auto resolve(const char* symbol) -> function_ptr;

  bool is_open() const;

  library(const library&) = delete;
  library& operator=(const library&) = delete;

private:
  std::unique_ptr<impl::library_impl> native_;
};

} // namespace ig

#endif // IG_ENVI_LIBRARY_H
