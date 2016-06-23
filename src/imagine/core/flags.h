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

#ifndef IG_CORE_FLAGS_H
#define IG_CORE_FLAGS_H

#include "imagine/ig.h"

namespace ig {

template <typename E>
class flags {
public:
  using enum_t       = E;
  using underlying_t = typename std::make_unsigned_t<
                       typename std::underlying_type_t<enum_t> >;

  constexpr flags() = default;
  constexpr flags(enum_t e) : flags_{underlying_t(e)} {}
  constexpr flags(underlying_t flags) : flags_{flags} {}

  auto operator~() const { return flags{~flags_}; }
  auto operator&(flags f) const { return flags{flags_ & f.flags_}; }
  auto operator|(flags f) const { return flags{flags_ | f.flags_}; }
  auto operator^(flags f) const { return flags{flags_ ^ f.flags_}; }

  auto operator&(enum_t e) const { return operator&(flags{e}); }
  auto operator|(enum_t e) const { return operator|(flags{e}); }
  auto operator^(enum_t e) const { return operator^(flags{e}); }

  auto& operator&=(flags f) { flags_ &= f.flags_; return *this; }
  auto& operator|=(flags f) { flags_ |= f.flags_; return *this; }
  auto& operator^=(flags f) { flags_ ^= f.flags_; return *this; }

  auto& operator&=(enum_t e) { return operator&=(flags{e}); }
  auto& operator|=(enum_t e) { return operator|=(flags{e}); }
  auto& operator^=(enum_t e) { return operator^=(flags{e}); }

  auto& operator=(flags f) { flags_ = f.flags_; return *this; }

  constexpr operator bool() const  { return flags_ != 0; }
  constexpr auto operator!() const { return !flags_; }

private:
  friend auto operator|(E lhs, flags rhs) { return rhs | lhs; }
  friend auto operator&(E lhs, flags rhs) { return rhs & lhs; }
  friend auto operator^(E lhs, flags rhs) { return rhs ^ lhs; }

  friend auto operator|(E lhs, E rhs) { return flags{lhs} | rhs; }
  friend auto operator&(E lhs, E rhs) { return flags{lhs} & rhs; }
  friend auto operator^(E lhs, E rhs) { return flags{lhs} ^ rhs; }

  underlying_t flags_;
};

template <typename E>
constexpr auto to_f(E e) {
  return static_cast< std::underlying_type_t<E> >(e);
}

} // namespace ig

#endif // IG_CORE_FLAGS_H
