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

#ifndef IG_MATH_DATA_H
#define IG_MATH_DATA_H

#include "imagine/math/bridge/bridge.h"

#include <vector>
#include <map>

namespace ig {

template <typename T>
class data {
public:
  using dimensions_type = std::initializer_list<uint32_t>;
  using data_type = data<T>;
  enum format_t : int32_t { unknown = -1, jpeg, png, bound = png + 1 };

  data() = default;
  explicit data(dimensions_type dimensions, uint32_t channels);

  auto buffer() const { return buffer_.data(); }
  auto buffer()       { return buffer_.data(); }

  auto& dimensions() const { return dimensions_; }
  auto& channels() const   { return channels_; }
  auto& pitch() const      { return pitch_; }
  
  const T& operator[](dimensions_type i) const { return buffer_[index(i)]; }
        T& operator[](dimensions_type i)       { return buffer_[index(i)]; }

protected:
  size_t index(dimensions_type i) const;

protected:
  std::vector<uint32_t> dimensions_;
  std::vector<T> buffer_;
  uint32_t channels_, size_, pitch_;

  friend bridge<data_type>;
  IG_API static std::map<int32_t, typename bridge<data_type>::type_t> bridge_table_;
};

template <typename T>
data<T>::data(dimensions_type dimensions, uint32_t channels)
  : dimensions_{dimensions}
  , channels_{channels}
  , size_{1} {

  pitch_ = dimensions_.front();
  for (auto it = dimensions_.begin() + 1; it != dimensions_.end(); ++it)
    size_ *= *it;

  size_ *= pitch_;
  buffer_.resize(size_ * channels_);
}

template <typename T>
size_t data<T>::index(dimensions_type i) const {
  size_t acc = 0;
  auto it = i.end();
  for (auto dim = dimensions_.size(); dim--> 1; )
    acc += *--it * std::accumulate(dimensions_.begin(), dimensions_.begin() + dim, 1, std::multiplies<>{});

  return *--it + acc;
}

using data8_t  = data<uint8_t>;
using data16_t = data<uint16_t>;

} // namespace ig

#endif // IG_MATH_DATA_H
