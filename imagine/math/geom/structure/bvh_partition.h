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

#ifndef IG_MATH_BVH_PARTITION_H
#define IG_MATH_BVH_PARTITION_H

#include "imagine/math/geom/spatial/aabb.h"

namespace ig {

template <typename Bounding>
class bvh_partition {
public:
  enum axis { 
    x = 0, 
    y = 1, 
    z = 2, dimensions = 3 };
  using sorted_dimension = std::vector<size_t>;

  explicit bvh_partition(const std::vector<Bounding>& boundings);

  size_t sort(uint32_t dimension, size_t begin, size_t end, size_t pivot);
  auto compute_bounding(size_t begin, size_t end) const -> Bounding;

protected:
  const std::vector<Bounding>& boundings_;
  size_t size_;

  std::vector<size_t> temporary_;
  std::vector<uint8_t> tags_;
  std::array<sorted_dimension, dimensions> indices_;
};

template <typename Bounding>
bvh_partition<Bounding>::bvh_partition(const std::vector<Bounding>& boundings)
  : boundings_{boundings}
  , size_{boundings_.size()}
  , temporary_(size_)
  , tags_(size_) {

  for (uint32_t d = 0; d < dimensions; ++d) {
    auto& indices = indices_[d]; 
          indices.resize(size_);

    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [this, &d](auto& lhs, auto& rhs) {
      auto& lb = boundings_[lhs];
      auto& rb = boundings_[rhs];
      return lb.min[d] + lb.max[d] < rb.min[d] + rb.max[d];
    });
  }
}

template <typename Bounding>
size_t bvh_partition<Bounding>::sort(uint32_t dimension, size_t begin, size_t end, size_t pivot) {
  enum { 
    left_region  = 0, 
    right_region = 1 };
  auto& split = indices_[dimension];

  for (size_t i = begin; i < pivot; ++i) tags_[split[i]] = left_region;
  for (size_t i = pivot; i < end;   ++i) tags_[split[i]] = right_region;

  for (uint32_t d = 0; d < dimensions; ++d) {
    if (d != dimension) continue;
    else {
      auto& indices = indices_[d];
      auto left  = begin;
      auto right = pivot;

      for (size_t i = begin; i < end; ++i) {
        auto id = indices[i];
        auto& ta = tags_[id] == left_region
          ? left
          : right;
        temporary_[ta++] = id;
      }

      if (end - begin > size_ / 2) {
        for (size_t i = 0;   i < begin; ++i) temporary_[i] = indices[i];
        for (size_t i = end; i < size_; ++i) temporary_[i] = indices[i];
        temporary_.swap(indices);
      } else for (size_t i = begin; i < end; ++i) temporary_[i] = indices[i];
    }
  } return pivot;
}

template <typename Bounding>
auto bvh_partition<Bounding>::compute_bounding(size_t begin, size_t end) const -> Bounding {
  Bounding bounding;
  for (size_t i = begin; i < end; ++i) bounding = union_g(bounding, boundings_[indices_[0][i]]);
  return bounding; 
}

} // namespace ig

#endif // IG_MATH_BVH_PARTITION_H
