/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYDYNAMIC_H
#define IG_MATH_NDARRAYDYNAMIC_H

#include "imagine/math/basis.h"
#include <vector>

namespace ig {
namespace nd {

template <typename T>
class dynamic_storage {
public:
  using value_type = T;
  using shape_type     = std::vector<size_t>;
  using container_type = std::vector<value_type>;
  friend class ndarray<T>;

  explicit dynamic_storage(const shape_type& shape, value_type value = {})
    : shape_{shape}
    , strides_(shape.size())
    , buffer_(striding(), value) {}

  void reshape(const shape_type& shape);

  auto dims() const { return shape_.size(); }
  auto size() const { return buffer_.size(); }

  auto buffer() const { return buffer_.data(); }
  auto buffer()       { return buffer_.data(); }

  decltype(auto) operator()(size_t n) const { return buffer_[n]; }
  decltype(auto) operator()(size_t n)       { return buffer_[n]; }

  auto operator[](uint32_t dimension) const
  { return shape_[dimension]; }

protected:
  auto striding();

  template <size_t C = 0, typename Id, typename... Ids> auto offset(Id i, Ids... is) const;
  template <size_t C>
  auto offset() const { return 0; }
  auto& shape() const { return shape_; }

private:
  shape_type shape_;
  shape_type strides_;
  container_type buffer_;
};

template <typename T>
template
< size_t C,
  typename Id,
  typename... Ids >
auto dynamic_storage<T>::offset(Id i, Ids... is) const {
  assert(
    C < dims() &&
    i < size() && "Invalid ndarray subscript");

  return
    strides_[C] * i +
    offset
    <C + 1>
      (is...);
}

template <typename T>
void dynamic_storage<T>::reshape(const shape_type& shape) {
  shape_ = shape;
  buffer_.resize(striding());
}

template <typename T>
auto dynamic_storage<T>::striding() {
  strides_.clear();
  return std::accumulate(
    shape_.begin(),
    shape_.end(),
    1, [this](size_t size, uint32_t dimension) { return strides_.emplace_back(size) * dimension; });
}

} // namespace nd
} // namespace ig

#endif // IG_MATH_NDARRAYDYNAMIC_H
