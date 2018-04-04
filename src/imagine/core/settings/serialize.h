/*
 Imagine v0.1
 [core]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_CORE_SERIALIZE_H
#define IG_CORE_SERIALIZE_H

#include "imagine/ig.h"

namespace ig {

template <typename T>
inline size_t hash_combine(size_t seed, const T& v) {
  std::hash<T> hasher;
  return seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

} // namespace ig

#endif // IG_CORE_SERIALIZE_H
