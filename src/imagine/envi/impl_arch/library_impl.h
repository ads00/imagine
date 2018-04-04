/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_LIBRARY_IMPL_H
#define IG_ENVI_LIBRARY_IMPL_H

#include "imagine/envi/impl_arch/config_impl.h"

namespace ig   {
namespace impl {

class library_impl {
public:
  library_impl();
  library_impl(const std::string& path);
  ~library_impl() = default;

  std::string path_;

  #if defined(IG_WIN)
  HMODULE handle_;
  // kernel32
  // <windows.h>
  #elif defined(IG_UNIX)
  void* handle_;
  // dl
  // <dlfcn.h>
  #endif
};

} // namespace impl
} // namespace ig

#endif // IG_ENVI_LIBRARY_IMPL_H
