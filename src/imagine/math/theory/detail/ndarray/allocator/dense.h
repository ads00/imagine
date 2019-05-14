/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_NDARRAYDENSE_H
#define IG_MATH_NDARRAYDENSE_H

#include "imagine/math/basis.h"
#include <vector>
#include <array>

namespace ig {
namespace nd {

template <typename Alloc>
class dense {
public:
  using value_type     = typename Alloc::value_type;
  using shape_type     = typename Alloc::shape_type;
  using container_type = typename Alloc::container_type;
  template <typename T, size_t... D> 
  friend class ndarray;

  template <typename... Args>
  explicit constexpr dense(Args&&... args) { Alloc::construct(shape_, strides_, buffer_, std::forward<Args>(args)...); }

  auto dims() const { return shape_.size(); }
  auto size() const { return buffer_.size(); }

  auto buffer() const { return buffer_.data(); }
  auto buffer()       { return buffer_.data(); }

  decltype(auto) operator()(size_t n) const { return buffer_[n]; }
  decltype(auto) operator()(size_t n)       { return buffer_[n]; }

  auto operator[](size_t dimension) const
  { return shape_[dimension]; }

//protected:
  template <size_t C = 0, typename Id, typename... Ids> auto access(Id i, Ids... is) const;
  template <size_t C>
  auto access() const { return 0; }

  auto& shape() const   { return shape_; }
  auto& strides() const { return strides_; }

private:
  shape_type shape_;
  shape_type strides_;
  container_type buffer_;
};

template <typename Alloc>
template
< size_t C,
  typename Id,
  typename... Ids >
auto dense<Alloc>::access(Id i, Ids... is) const {
  assert(
    C < dims() &&
    i < size() && "Invalid ndarray subscript");

  return
    strides_[C] * i +
    access
    <C + 1>
      (is...);
}

template <typename T>
class dynamic_alloc {
public:
  using value_type = T;
  using shape_type     = std::vector<size_t>;
  using container_type = std::vector<value_type>;

  template <typename Shape>
  static constexpr 
  void construct(
    shape_type& shape, 
    shape_type& strides, 
    container_type& buffer, 
    Shape&& dims) {

    shape.assign(dims.begin(), dims.end());
    buffer.resize(
      std::accumulate(
        shape.begin(),
        shape.end(),
        1,
        [&](auto size, auto dim) 
        { return strides.emplace_back(size) * dim; })
      );
  }
};

template <typename T, size_t... D>
class static_alloc {
public:
  using value_type = T;
  using shape_type     = std::array<size_t, sizeof...(D)>;
  using container_type = std::array<value_type, (D * ... * 1)>;

  template <typename... Args>
  static constexpr
  void construct(
    shape_type& shape, 
    shape_type& strides, 
    container_type& buffer, 
    Args&&... values) {

    shape = {D...};
    strides[0] = 1;
    std::transform(
      shape.begin(),
      shape.end() - 1,
      strides.begin(),
      strides.begin() + 1,
      [](auto dim, auto stride) 
      { return dim * stride; });
    buffer = {{std::forward<Args>(values)...}};
  }
};

} // namespace nd
} // namespace ig

#endif // IG_MATH_NDARRAYDENSE_H
