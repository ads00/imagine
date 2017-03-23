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

#ifndef IG_MATH_NDARRAY_H
#define IG_MATH_NDARRAY_H

#include "imagine/math/theory/detail/bridge.h"

#include <array>
#include <vector>

namespace ig {

template <typename T>
class ndarray {
public:
  using value_type = std::vector<T>;
  using shape_type = std::vector<uint32_t>;

  ndarray() = default;
  explicit ndarray(shape_type shape, uint32_t features = 1);

  auto buffer() const { return buffer_.data(); }
  auto buffer()       { return buffer_.data(); }

  auto& get_shape() const    { return shape_; }
  auto& get_features() const { return features_; }

protected:
  size_t index(shape_type s) const;

protected:
  value_type buffer_;
  shape_type shape_;
  uint32_t features_, size_;
};

template <typename T>
ndarray<T>::ndarray(shape_type shape, uint32_t features)
  : shape_{shape}
  , features_{features}
  , size_{1} {

  for (auto dimension : shape)
    size_ *= dimension;
  buffer_.resize(size_ * features_);
}

template <typename T>
size_t ndarray<T>::index(shape_type s) const {
  assert(shape_.size() == s.size() && "Incoherent array shape");

  size_t index = 0;
  auto   total = size_;
  for (size_t i = shape_.size(); i--> 0;) {
    total /= shape_[i];
    index += total * s[i];
  }
  return index * features_;
}

enum class set_format { };
enum class image_format { jpeg, bmp, png, hdr, pam };
enum class sound_format { flag, mp3, ogg, wav };

template <typename T> using train_bridge = bridge < ndarray<T>, set_format>;
template <typename T> using image_bridge = bridge < ndarray<T>, image_format>;
template <typename T> using sound_bridge = bridge < ndarray<T>, sound_format>;

} // namespace ig

#endif // IG_MATH_NDARRAY_H
