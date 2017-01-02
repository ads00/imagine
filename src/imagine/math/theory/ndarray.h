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

#ifndef IG_MATH_NDARRAY_H
#define IG_MATH_NDARRAY_H

#include "imagine/math/theory/detail/bridge.h"

#include <array>
#include <vector>
#include <map>

namespace ig {

template <typename T, size_t N>
class ndarray {
public:
  using shape_type = std::array<uint32_t, N>;
  using value_type = std::vector<T>;

  ndarray() = default;
  explicit ndarray(shape_type shape, uint32_t features = 1);

  auto buffer() const { return buffer_.data(); }
  auto buffer()       { return buffer_.data(); }

  auto& get_shape() const    { return shape_; }
  auto& get_features() const { return features_; }

  struct slice {
    explicit slice(T* s, ndarray& a) 
      : sbuffer{s}
      , a{a} {}
    const T& f(uint32_t i) const { assert(i < a.get_features() && "Invalid array slice subscript"); return *(sbuffer + i); }
          T& f(uint32_t i)       { assert(i < a.get_features() && "Invalid array slice subscript"); return *(sbuffer + i); }
    T* sbuffer; ndarray& a;
  };
  
  const auto operator[](shape_type s) const { return slice{&buffer_[index(s)], *this}; }
        auto operator[](shape_type s)       { return slice{&buffer_[index(s)], *this}; }

protected:
  size_t index(shape_type s) const;

protected:
  value_type buffer_;
  shape_type shape_;
  uint32_t features_, size_;

  friend bridge<ndarray>;
  IG_API static std::map<int32_t, typename bridge<ndarray>::gen_t> bridge_table_;
};

template <typename T, size_t N>
ndarray<T, N>::ndarray(shape_type shape, uint32_t features)
  : shape_{shape}
  , features_{features}
  , size_{1} {

  for (auto dimension : shape)
    size_ *= dimension;
  buffer_.resize(size_ * features_);
}

template <typename T, size_t N>
size_t ndarray<T, N>::index(shape_type s) const {
  size_t index = 0;
  auto   total = size_;
  for (size_t i = 0; i < N; ++i) {
    total /= shape_[i];
    index += total * s[i];
  }
  return index * features_;
}

template <typename T, size_t N> 
using tensor = 
  ndarray<T, N>;

template <typename T> using array1d = ndarray<T, 1>;
template <typename T> using array2d = ndarray<T, 2>;

} // namespace ig

#endif // IG_MATH_NDARRAY_H
