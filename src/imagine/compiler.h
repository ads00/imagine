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

#ifndef COMPILER_H
#define COMPILER_H

/*
MSVC     - Microsoft Visual C/C++, Intel C++ for Windows
GNU      - GNU C++
INTEL    - Intel C++ for Linux, Intel C++ for Windows
CLANG    - C++ front-end for the LLVM compiler
*/

#if defined(_MSC_VER)
# define IG_MSVC (_MSC_VER)
# define IG_FUNC __FUNCTION__
# define IG_DEPRECATED __declspec(deprecated)
# define IG_DEPRECATED_X(text) __declspec(deprecated(text))
# define IG_NORETURN __declspec(noreturn)
# define IG_TLS __declspec(thread)
# define IG_EXPORT __declspec(dllexport)
# define IG_IMPORT __declspec(dllimport)

# pragma warning(disable:4251)

# if defined(__INTEL_COMPILER)
#  define IG_INTEL
# endif

#elif defined(__ARMCC__) || defined(__CC_ARM)
# define IG_RVCT
# define IG_DEPRECATED __attribute__((__deprecated__))
# if defined(IG_LINUX)
#  define IG_EXPORT __attribute__((visibility("default")))
#  define IG_IMPORT __attribute__((visibility("default")))
# else
#  define IG_EXPORT __declspec(dllexport)
#  define IG_IMPORT __declspec(dllimport)
# endif

#elif defined(__GNUC__)
# define IG_GNU (__GNUC__ * 100 + __GNUC_MINOR__)
# if defined(__MINGW32)
#  define IG_MINGW
# endif
# if defined(__INTEL_COMPILER)
#  define IG_INTEL (__INTEL_COMPILER)
#   if defined(__clang__)
#    define IG_CLANG 305
#   endif
# elif defined(__clang__)
#  define IG_CLANG ((__clang_major__ * 100) + __clang_minor__)
# else
#  if IG_GNU >= 405
#   define IG_DEPRECATED_X(text) __attribute__((__deprecated__(text)))
#  endif
# endif
# if defined(IG_WIN)
#  define IG_EXPORT __declspec(dllexport)
#  define IG_IMPORT __declspec(dllimport)
# elif defined(IG_UNIX)
#  define IG_EXPORT __attribute__((visibility("default")))
#  define IG_IMPORT __attribute__((visibility("default")))
# endif
# define IG_FUNC __PRETTY_FUNCTION__
# define IG_DEPRECATED __attribute__((__deprecated__))
# define IG_UNUSED __attribute__((__unused__))
# define IG_NORETURN __attribute__((__noreturn__))
# define IG_TLS __thread
#else
# error "Imagine doesn't support this compiler"
#endif

#if defined(IG_SHARED) || !defined(IG_STATIC)
# if defined(IG_STATIC)
#  error "Both IG_SHARED and IG_STATIC defined, ambigous compilation"
# endif
# if !defined(IG_SHARED)
#  define IG_SHARED
# endif
# if defined(IG_BUILD)
#  define IG_API IG_EXPORT
# else
#  define IG_API IG_IMPORT
# endif
#else
# define IG_API
#endif

#endif // COMPILER_H
