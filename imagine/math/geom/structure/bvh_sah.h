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

#ifndef IG_MATH_BVH_SAH_H
#define IG_MATH_BVH_SAH_H

#include "imagine/math/geom/structure/bvh_partition.h"

namespace ig {

template <typename Tree, typename Bounding = typename Tree::bounding_type>
class bvh_sah : public bvh_partition<Bounding> {
public:
  using partition_type = bvh_partition<Bounding>;
  using partition_type::sort;
  using partition_type::compute_bounding;
  using partition_type::boundings_; using partition_type::indices_;

  explicit bvh_sah(const std::vector<Bounding>& boundings, size_t leaf_nodes, float traversal_cost = 1.f, float intersect_cost = 1.f)
    : bvh_partition<Bounding>{boundings}
    , leaves_{leaf_nodes}
    , traversal_{traversal_cost}
    , intersect_{intersect_cost}
    , left_areas_(boundings.size() > 1 ? boundings.size() - 1 : 0) {}

  void build(Tree& tree);

protected:
  void subdivide(Tree& tree, size_t id, size_t begin, size_t end, const Bounding& bounding);
  auto partition(size_t begin, size_t end, const Bounding& bounding) -> size_t;

private:
  size_t leaves_;
  float traversal_;
  float intersect_;
  std::vector<float> left_areas_;
};

template <typename Tree, typename Bounding>
void bvh_sah<Tree, Bounding>::build(Tree& tree) {
  auto size = boundings_.size();
  auto root = compute_bounding(0, size);

  tree.clear();
  tree.nodes_.reserve(2 * size / leaves_ - 1);
  tree.nodes_.emplace_back(root);
  tree.ordering_ = indices_[0];

  subdivide(tree, 0, 0, size, root);
}

template <typename Tree, typename Bounding>
void bvh_sah<Tree, Bounding>::subdivide(Tree& tree, size_t id, size_t begin, size_t end, const Bounding& bounding) {
  auto pivot = end;
  auto& current = tree.nodes_[id];
  if (end - begin > 1) pivot = partition(begin, end, bounding);
  if (pivot == end)    current.make_leaf(begin, static_cast<uint32_t>(end - begin)); 
  else {
    auto left  = compute_bounding(begin, pivot);
    auto right = compute_bounding(pivot, end);

    auto left_index = tree.nodes_.size(), right_index = left_index + 1;
    current.make_interior(left_index);

    tree.nodes_.emplace_back(left);
    tree.nodes_.emplace_back(right);

    subdivide(tree, left_index, begin, pivot, left);
    subdivide(tree, right_index, pivot, end, right);
  }
}

template <typename Tree, typename Bounding>
auto bvh_sah<Tree, Bounding>::partition(size_t begin, size_t end, const Bounding& bounding) -> size_t {
  auto count = end - begin;
  if (count <= leaves_)
    return end;

  auto cost = std::numeric_limits<float>::max();
  size_t pivot = 0;
  uint32_t split = 0;

  for (uint32_t d = 0; d < dimensions; ++d) {
    auto& indices = indices_[d];

    Bounding left;
    for (size_t i = 0; i < count - 1; ++i) left = union_g(left, boundings_[indices[begin + i]]), left_areas_[i] = left.half_area();

    Bounding right;
    for (size_t i = count; i-- > 0; ) {
      right = union_g(right, boundings_[indices[begin + i]]);
      auto split_cost = left_areas_[i - 1] * i + right.half_area() * (count - i);

      if (cost > split_cost) {
        cost = split_cost;
        pivot = i;
        split = d; 
      }
    }
  }

  auto split_cost = traversal_ + cost / bounding.half_area() * intersect_;
  auto leafy_cost = count * intersect_;
  if (leafy_cost <= split_cost)
    return end;
  return sort(split, begin, end, begin + pivot);
}

} // namespace ig

#endif // IG_MATH_BVH_SAH_H
