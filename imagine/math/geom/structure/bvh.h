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

#ifndef IG_MATH_BVH_H
#define IG_MATH_BVH_H

#include "imagine/math/geom/proj.h"
#include "imagine/math/geom/structure/bvh_partition.h"
#include "imagine/math/geom/structure/bvh_sah.h"

#include <stack>

namespace ig {

template <typename Object, typename Bounding>
class bvh {
public:
  using object_type = Object;
  using bounding_type = Bounding;

  template <typename Tree, typename Bounding> friend class bvh_sah;
  template <typename Tree, typename Bounding, typename Item> friend class bvh_spatial;

  explicit bvh(const std::vector<object_type>& objects)
    : objects_{objects} {}

  void clear() { nodes_.clear(); }

  template <typename Callable>
  bool traverse(const ray3& ray, float distance, Callable&& fn);

  bool occlu(const ray3& ray, float thit) 
  { return traverse(ray, thit, [&](auto& obj, auto& distance, auto& hit) { return !obj.occlu(ray, distance); }); }

  template <typename Info, typename Visitor>
  bool query(const ray3& ray, Info& info, Visitor&& visitor) {
    auto hit = traverse(ray, info.distance, [&](auto& obj, auto& distance, auto& hit) {
      if (obj.query(ray, distance, info)) {
        visitor.visit(obj);
        hit = true;
      } return true; 
    }); return hit
          ? visitor.process(ray)
          : hit;
  }

private:
  std::vector<size_t> ordering_;
  std::vector<object_type> objects_;

  struct node; 
  std::vector<node> nodes_; 
};

template <typename Object, typename Bounding>
struct bvh<Object, Bounding>::node {
  node() = default;
  explicit node(const bounding_type& bound) 
    : bounding{bound}
    , index{0}
    , items{0} {}

  void make_leaf(size_t id, uint32_t nodes) { index = id; items = nodes; }
  void make_interior(size_t id)             { index = id; items = ~0; }
    
  bool leaf() const     { return items != ~0; }
  bool interior() const { return items == ~0; }

  bounding_type bounding;
  size_t index; uint32_t items; };

template <typename Object, typename Bounding>
template <typename Callable>
bool bvh<Object, Bounding>::traverse(const ray3& ray, float distance, Callable&& fn) {
  bool hit = false;
  vec3 dir_rcp = 1.f / ray.direction;

  std::stack
    < std::add_pointer_t<node>
    > stack{};
  stack.emplace(&nodes_[0]);

  while (true) {
    auto node = stack.top();
    stack.pop();
    if (node->interior()) {
      auto left_index = node->index, right_index = node->index + 1;

      float tn0, tn1, tf0, tf1;
      bool hit_left  = nodes_[left_index] .bounding.intersect(ray.origin, dir_rcp, tn0, tf0) && tn0 < distance;
      bool hit_right = nodes_[right_index].bounding.intersect(ray.origin, dir_rcp, tn1, tf1) && tn1 < distance;

      if (hit_left)  stack.emplace(&nodes_[left_index]);
      if (hit_right) stack.emplace(&nodes_[right_index]);
    } else {
      auto id = node->index;
      for (auto obj = node->items; obj--; id++) {
        auto proceed = fn(objects_[ordering_[id]], distance, hit);
        if (!proceed) return true;
      }
    } if (stack.empty()) break;
  } return hit;
}

} // namespace ig

#endif // IG_MATH_BVH_H
