/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_CONFIG_IMPL_H
#define IG_ENVI_CONFIG_IMPL_H

#include "imagine/ig.h"
#if defined(IG_WIN)
 #undef UNICODE
 #define ISOLATION_AWARE_ENABLED 1
 #define WIN32_LEAN_AND_MEAN
 #define VC_EXTRALEAN

 #include <windows.h>
 #include <commctrl.h>

 #pragma comment(lib, "comctl32.lib")
 #pragma comment(linker,"\"/manifestdependency:type='win32' \
 name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
 processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#endif // IG_ENVI_CONFIG_IMPL_H
