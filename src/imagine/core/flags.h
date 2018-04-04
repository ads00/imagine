/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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
