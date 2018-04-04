/*
 Imagine v0.1
 [root]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_SUP_H
#define IG_SUP_H

/*
GNU      - GNU C++
CLANG    - C++ front-end for the LLVM compiler
*/

#if defined(__GNUC__)
# define IG_GNU (__GNUC__ * 100 + __GNUC_MINOR__)
# if defined(__clang__)
#  define IG_CLANG ((__clang_major__ * 100) + __clang_minor__)
# endif
# if defined(__MINGW32)
#  define IG_MINGW
# endif
# if defined(IG_WIN)
#  define IG_EXPORT __declspec(dllexport)
#  define IG_IMPORT __declspec(dllimport)
# elif defined(IG_UNIX)
#  define IG_EXPORT __attribute__((visibility("default")))
#  define IG_IMPORT __attribute__((visibility("default")))
# endif
# define IG_DEPRECATED __attribute__((__deprecated__))
# define IG_UNUSED __attribute__((__unused__))
# define IG_NORETURN __attribute__((__noreturn__))
# define IG_TLS __thread

#else
# error "Imagine doesn't support this compiler"
#endif

#if defined(IG_SHARED) || !defined(IG_STATIC)
# if defined(IG_STATIC)
#  error "Both ig_shared and ig_static defined, ambigous compilation"
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

#endif // IG_SUP_H
