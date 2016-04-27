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

#ifndef IG_PLATFORM_H
#define IG_PLATFORM_H

/*
DARWIN   - Any Darwin system
MAC      - OS X and iOS
OSX      - OS X
MSDOS    - MS-DOS and Windows
WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
WINCE    - WinCE (Windows CE 5.0)
WINRT    - WinRT (Windows 8 Runtime)
CYGWIN   - Cygwin
LINUX    - Linux [has variants]
FREEBSD  - FreeBSD [has variants]
BSD4     - Any BSD 4.4 system
UNIX     - Any UNIX BSD/SYSV system
ANDROID  - Android platform
*/

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
# define IG_DARWIN
# define IG_BSD4

# if defined(_LP64_)
#  define IG_DARWIN64
# else
#  define IG_DARWIN32
# endif

#elif defined(ANDROID)
# define IG_ANDROID
# define IG_LINUX

#elif defined(__CYGWIN__)
# define IG_CYGWIN

#elif (!defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && \
        defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
# define IG_WIN32
# define IG_WIN64

#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
# if defined(WINCE) || defined(_WIN32_WCE)
#  define IG_WINCE
# elif defined(WINAPI_FAMILY)
#  if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#   define IG_WINPHONE
#   define IG_WINRT
#  elif WINAPI_FAMILY == WINAPI_FAMILY_APP
#   define IG_WINRT
#  else
#   define IG_WIN32
#  endif
# else
#  define IG_WIN32
# endif

#elif defined(__linux__) || defined(__linux)
# define IG_LINUX

#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
# ifndef __FreeBSD_kernel__
#  define IG_FREEBSD
# endif
# define IG_FREEBSD_KERNEL
# define IG_BSD4

#else
# error "Imagine doesn't support this platform"
#endif

#if defined(IG_WIN32) || defined(IG_WIN64) || defined(IG_WINCE) || defined(IG_WINRT)
# define IG_WIN
#endif

#if defined(IG_DARWIN)
# define IG_MAC
# if defined(IG_DARWIN64)
#  define IG_MAC64
# elif defined(IG_DARWIN32)
#  define IG_MAC32
# endif
# include <TargetConditionals.h>
# if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#  define IG_IOS
# elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
#  define IG_OSX
#  define IG_MACX
# endif
#endif

#if defined(IG_WIN)
# undef IG_UNIX
#elif !defined(IG_UNIX)
# define IG_UNIX
#endif

#endif // IG_PLATFORM_H
