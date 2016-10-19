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

#include "imagine/math/processing/data.h"

#include <algorithm>
#include <numeric>

namespace ig {

data::data(dimensions_type dimensions, uint32_t channels)
  : dims_{dimensions}
  , channels_{channels}
  , size_{1} {

  pitch_ = dims_.front();
  for (auto it = dims_.begin() + 1; it != dims_.end(); ++it)
    size_ *= *it;

  size_ *= pitch_;
  ptr_.resize(size_ * channels_);
}

const uint8_t& data::operator[](dimensions_type coords) const {
  size_t acc = 0; 
  auto it = coords.end();
  for (auto dim = dims_.size(); dim--> 1; )
    acc += *--it * std::accumulate(dims_.begin(), dims_.begin() + dim, 1, std::multiplies<>{});

  auto id = *--it + acc;
  return ptr_[id];
}

uint8_t& data::operator[](dimensions_type coords) {
  size_t acc = 0;
  auto it = coords.end();
  for (auto dim = dims_.size(); dim--> 1; )
    acc += *--it * std::accumulate(dims_.begin(), dims_.begin() + dim, 1, std::multiplies<>{});

  auto id = *--it + acc;
  return ptr_[id];
}

} // namespace ig
