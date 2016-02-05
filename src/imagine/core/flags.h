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

#ifndef CORE_FLAGS_H
#define CORE_FLAGS_H

#include "imagine.h"

namespace ig
{

template <typename TEnum>
class flags
{
public:
  using enum_t       = TEnum;
  using underlying_t = typename std::make_unsigned<
                       typename std::underlying_type< enum_t>::type >::type;

  constexpr flags() = default;
  constexpr flags(enum_t e) : flags_{underlying_t(e)} {}

  flags operator~() const { return flags(~flags_); }
  flags operator&(flags f) const { return flags(flags_ & f.flags_); }
  flags operator|(flags f) const { return flags(flags_ | f.flags_); }
  flags operator^(flags f) const { return flags(flags_ ^ f.flags_); }

  flags operator&(enum_t e) const { return operator&(flags(e)); }
  flags operator|(enum_t e) const { return operator|(flags(e)); }
  flags operator^(enum_t e) const { return operator^(flags(e)); }

  flags& operator&=(flags f) { flags_ &= f.flags_; return *this; }
  flags& operator|=(flags f) { flags_ |= f.flags_; return *this; }
  flags& operator^=(flags f) { flags_ ^= f.flags_; return *this; }

  flags& operator&=(enum_t e) { return operator&=(flags(e)); }
  flags& operator|=(enum_t e) { return operator|=(flags(e)); }
  flags& operator^=(enum_t e) { return operator^=(flags(e)); }

  flags& operator=(flags f) { flags_ = f.flags_; return *this; }

  constexpr operator bool() const  { return flags_ != 0; }
  constexpr bool operator!() const { return !flags_; }

private:
  constexpr flags(underlying_t flags) : flags_{flags} {}

  friend flags operator|(TEnum f1, flags f2) { return f2 | f1; }
  friend flags operator&(TEnum f1, flags f2) { return f2 & f1; }
  friend flags operator^(TEnum f1, flags f2) { return f2 ^ f1; }

  friend flags operator|(TEnum f1, TEnum f2) { return flags(f1) | f2; }
  friend flags operator&(TEnum f1, TEnum f2) { return flags(f1) & f2; }
  friend flags operator^(TEnum f1, TEnum f2) { return flags(f1) ^ f2; }

  underlying_t  flags_;
};

} // namespace ig

#endif // CORE_FLAGS_H
