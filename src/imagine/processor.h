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

#ifndef PROCESSOR_H
#define PROCESSOR_H

#if defined(__ORDER_BIG_ENDIAN__)
# define IG_BIG_ENDIAN __ORDER_BIG_ENDIAN__
#else
# define IG_BIG_ENDIAN 4321
#endif
#if defined(__ORDER_LITTLE_ENDIAN__)
# define IG_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#else
# define IG_LITTLE_ENDIAN 1234
#endif

#if defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(__arm64__)
# define IG_ARM
# if defined(__arm64__)
#  define IG_ARM_64
# else
#  define IG_ARM_32
# endif
# if defined(__ARM64_ARCH_8__)
#  define IG_ARM_V8
#  define IG_ARM_V7
#  define IG_ARM_V6
#  define IG_ARM_V5
# elif defined(__ARM_ARCH_7__) \
  || defined(__ARM_ARCH_7A__)  \
  || defined(__ARM_ARCH_7R__)  \
  || defined(__ARM_ARCH_7M__)  \
  || defined(__ARM_ARCH_7S__)  \
  || defined(_ARM_ARCH_7) \
  || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7) \
  || (defined(_M_ARM) && _M_ARM-0 >= 7)
#  define IG_ARM_V7
#  define IG_ARM_V6
#  define IG_ARM_V5
# elif defined(__ARM_ARCH_6__) \
  || defined(__ARM_ARCH_6J__)  \
  || defined(__ARM_ARCH_6T2__) \
  || defined(__ARM_ARCH_6Z__)  \
  || defined(__ARM_ARCH_6K__)  \
  || defined(__ARM_ARCH_6ZK__) \
  || defined(__ARM_ARCH_6M__)  \
  || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6) \
  || (defined(_M_ARM) && _M_ARM-0 >= 6)
#  define IG_ARM_V6
#  define IG_ARM_V5
# elif defined(__ARM_ARCH_5TEJ__) \
  || defined(__ARM_ARCH_5TE__) \
  || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5) \
  || (defined(_M_ARM) && _M_ARM-0 >= 5)
#  define IG_ARM_V5
# endif
# if defined(__ARMEL__)
#  define IG_BYTE_ORDER IG_LITTLE_ENDIAN
# elif defined(__ARMEB__)
#  define IG_BYTE_ORDER IG_BIG_ENDIAN
# endif

#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
# define IG_X86_32
# define IG_BYTE_ORDER IG_LITTLE_ENDIAN

# if defined(_M_IX86)
#  define IG_X86     (_M_IX86/100)
# elif defined(__i686__) || defined(__athlon__) || defined(__SSE__)
#  define IG_X86     6
# elif defined(__i586__) || defined(__k6__)
#  define IG_X86     5
# elif defined(__i486__)
#  define IG_X86     4
# else
#  define IG_X86     3
# endif

#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
# define IG_X86      6
# define IG_X86_64
# define IG_BYTE_ORDER IG_LITTLE_ENDIAN

#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
# define IG_IA64

#elif defined(__mips) || defined(__mips__) || defined(_M_MRX000)
# define IG_MIPS
# if defined(_MIPS_ARCH_MIPS1) || (defined(__mips) && __mips - 0 >= 1)
#  define IG_MIPS_I
# endif
# if defined(_MIPS_ARCH_MIPS2) || (defined(__mips) && __mips - 0 >= 2)
#  define IG_MIPS_II
# endif
# if defined(_MIPS_ARCH_MIPS32) || defined(__mips32)
#  define IG_MIPS_32
# endif
# if defined(_MIPS_ARCH_MIPS3) || (defined(__mips) && __mips - 0 >= 3)
#  define IG_MIPS_III
# endif
# if defined(_MIPS_ARCH_MIPS4) || (defined(__mips) && __mips - 0 >= 4)
#  define IG_MIPS_IV
# endif
# if defined(_MIPS_ARCH_MIPS5) || (defined(__mips) && __mips - 0 >= 5)
#  define IG_MIPS_V
# endif
# if defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
#  define IG_MIPS_64
# endif
# if defined(__MIPSEL__)
#  define IG_BYTE_ORDER IG_LITTLE_ENDIAN
# elif defined(__MIPSEB__)
#  define IG_BYTE_ORDER IG_BIG_ENDIAN
# endif

#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__)   \
 || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC) \
 || defined(_M_MPPC) || defined(_M_PPC)
# define IG_POWER
# if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
#  define IG_POWER_64
# else
#  define IG_POWER_32
# endif
#endif

#if !defined(IG_BYTE_ORDER)
# if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == IG_BIG_ENDIAN || __BYTE_ORDER__ == IG_LITTLE_ENDIAN)
#  define IG_BYTE_ORDER __BYTE_ORDER__
# elif defined(__BIG_ENDIAN__) || defined(_big_endian__) || defined(_BIG_ENDIAN)
#  define IG_BYTE_ORDER IG_BIG_ENDIAN
# elif defined(__LITTLE_ENDIAN__) || defined(_little_endian__) || defined(_LITTLE_ENDIAN) \
  || defined(_WIN32_WCE) || defined(WINAPI_FAMILY)
#  define IG_BYTE_ORDER IG_LITTLE_ENDIAN
# else
#  error "Unable to determine byte order"
# endif
#endif

#endif // PROCESSOR_H
