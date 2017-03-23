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

#ifndef IG_CORE_FLAGS_H
#define IG_CORE_FLAGS_H

#include "imagine/ig.h"

namespace ig {

template <typename E>
class flags {
public:
  using enum_type       = E;
  using underlying_type = typename std::make_unsigned_t<
                          typename std::underlying_type_t<enum_type> >;

  constexpr flags() = default;
  constexpr flags(enum_type e) : flags_{underlying_type(e)} {}
  constexpr explicit flags(underlying_type flags) : flags_{flags} {}

  auto operator~() const { return flags{~flags_}; }
  auto operator&(enum_type e) const { return operator&(flags{e}); }
  auto operator|(enum_type e) const { return operator|(flags{e}); }
  auto operator^(enum_type e) const { return operator^(flags{e}); }
  auto operator&(const flags& f) const { return flags{flags_ & f.flags_}; }
  auto operator|(const flags& f) const { return flags{flags_ | f.flags_}; }
  auto operator^(const flags& f) const { return flags{flags_ ^ f.flags_}; }

  auto& operator&=(const flags& f) { flags_ &= f.flags_; return *this; }
  auto& operator|=(const flags& f) { flags_ |= f.flags_; return *this; }
  auto& operator^=(const flags& f) { flags_ ^= f.flags_; return *this; }

  auto& operator=(const flags& f) { flags_ = f.flags_; return *this; }
  auto& operator=(underlying_type flags) { flags_ = underlying_type(flags); return *this; }

  constexpr auto operator!() const { return !flags_; }
  constexpr operator underlying_type() const { return flags_; }
            operator underlying_type*()      { return &flags_; }

private:
  underlying_type flags_;
};

template <typename E> constexpr auto operator|(E lhs, E rhs) 
{ return flags<E>{lhs} | rhs; }
template <typename E> constexpr auto operator&(E lhs, E rhs) 
{ return flags<E>{lhs} & rhs; }
template <typename E> constexpr auto operator^(E lhs, E rhs)
{ return flags<E>{lhs} ^ rhs; }

template <typename E>
constexpr auto to_f(E e) { return static_cast< std::underlying_type_t<E> >(e); }

} // namespace ig

#endif // IG_CORE_FLAGS_H
