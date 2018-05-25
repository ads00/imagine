/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_CORE_REVERSE_H
#define IG_CORE_REVERSE_H

#include "imagine/ig.h"

namespace ig {

template <typename X, size_t... I>
auto reverse_impl(X&& x, std::index_sequence<I...>)
{
  return std::forward_as_tuple(std::get<sizeof...(I) - 1 - I>(std::forward<X>(x))...);
}

template <typename X>
auto reverse(X&& x)
{
  return reverse_impl(
    std::forward<X>(x), std::make_index_sequence<std::tuple_size<X>::value>());
}

} // namespace ig

#endif // IG_CORE_REVERSE_H
