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

template <typename Enum>
class flags {
public:
  using value_type = Enum;
  using underlying_type = typename std::underlying_type_t<value_type>;
  static_assert(std::is_enum<value_type>::value, "Flags require an enum type");

  constexpr flags() = default;
  constexpr flags(value_type e) : flags_{underlying_type(e)} {}
  constexpr explicit flags(underlying_type flags)
    : flags_{flags} {}

  auto operator~() const
  { return flags{~flags_}; }
  auto operator&(const flags& f) const
  { return flags{flags_ & f.flags_}; }
  auto operator|(const flags& f) const
  { return flags{flags_ | f.flags_}; }
  auto operator^(const flags& f) const
  { return flags{flags_ ^ f.flags_}; }

  auto operator&(value_type e) const { return operator&(flags{e}); }
  auto operator|(value_type e) const { return operator|(flags{e}); }
  auto operator^(value_type e) const { return operator^(flags{e}); }

  auto& operator&=(const flags& f) { flags_ &= f.flags_; return *this; }
  auto& operator|=(const flags& f) { flags_ |= f.flags_; return *this; }
  auto& operator^=(const flags& f) { flags_ ^= f.flags_; return *this; }
  auto& operator= (const flags& f) { flags_ =  f.flags_; return *this; }

  constexpr auto operator!() const { return !flags_; }
  constexpr operator underlying_type() const { return flags_; }
            operator underlying_type*()      { return &flags_; }

private:
  underlying_type flags_;
};

template <typename Enum> constexpr auto operator|(Enum lhs, Enum rhs)
{ return flags<Enum>{lhs} | rhs; }
template <typename Enum> constexpr auto operator&(Enum lhs, Enum rhs)
{ return flags<Enum>{lhs} & rhs; }
template <typename Enum> constexpr auto operator^(Enum lhs, Enum rhs)
{ return flags<Enum>{lhs} ^ rhs; }

template <typename Enum>
constexpr auto enum_cast(Enum e) { 
  return static_cast
    < std::underlying_type_t<Enum>
    >(e); 
}

} // namespace ig

#endif // IG_CORE_FLAGS_H
