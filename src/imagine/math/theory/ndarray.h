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

#include "imagine/math/theory/detail/ndarray/base.h"

namespace ig {

template <typename T>
struct ndarray_traits
<
  ndarray<T>
>
{
  using value_type = T;
};

template <typename t_>
class ndarray : public ndarray_base< ndarray<t_> > {
public:
  using value_type = t_;
  using index_type     = std::initializer_list<uint32_t>;
  using shape_type     = std::vector<uint32_t>;
  using container_type = std::vector<value_type>;

  ndarray() = default;
  explicit ndarray(index_type shape, const value_type& value = t_());

  auto& shape() const   { return shape_; }
  auto& strides() const { return strides_; }
  auto& dims() const    { return strides_.size(); }

  auto size() const   { return buffer_.size(); }
  auto buffer() const { return buffer_.data(); }
  auto buffer()       { return buffer_.data(); }

  void reshape(index_type shape);

  auto& operator[](index_type index) { return buffer_[offset_helper(index)]; }

protected:
  size_t striding();
  size_t offset_helper(index_type index) const;

protected:
  shape_type shape_, strides_;
  container_type buffer_;
};

template <typename t_>
ndarray<t_>::ndarray(index_type shape, const value_type& value)
  : shape_{shape}
  , strides_(shape_.size())
  , buffer_ (striding(), value) {}

template <typename t_>
void ndarray<t_>::reshape(index_type shape) {
  shape_ = shape;
  buffer_.resize(striding());
}

template <typename t_>
size_t ndarray<t_>::striding() {
  strides_.clear();
  return std::accumulate(
    shape_.begin(),
    shape_.end(),
    1,
    [this](size_t size, uint32_t dimension) { return strides_.emplace_back(size) * dimension; });
}

template <typename t_>
size_t ndarray<t_>::offset_helper(index_type index) const {
  return std::inner_product(
    strides_.begin(),
    strides_.end(),
    index.begin(),
    0);
}

} // namespace ig

#endif // IG_MATH_NDARRAY_H
