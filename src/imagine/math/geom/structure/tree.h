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

#ifndef IG_MATH_TREE_H
#define IG_MATH_TREE_H

#include "imagine/math/geom/proj.h"
#include "imagine/math/geom/structure/traversal.h"

namespace ig {
namespace builder { template <typename Hierarchy, typename Heuristic> class sah; }

template <typename Object>
class tree {
public:
  template <typename Hierarchy, typename Heuristic> friend class builder::sah;
  friend traversal;
  static constexpr size_t M = IG_PACKET_WIDE;

  struct ref {
    auto is_node() const { return count == uint32_t(~0); } 
    auto is_leaf() const { return count != uint32_t(~0); }
    uint32_t count, index; };

  using leaf = typename Object::primitive;
  struct node {
    using bounds = std::array<packet_float, 2>;

    void invalidate(size_t i) { 
      x.front()[i] = y.front()[i] = z.front()[i] =  std::numeric_limits<float>::infinity();
      x.back ()[i] = y.back ()[i] = z.back ()[i] = -std::numeric_limits<float>::infinity(); }
    void encode_packet(
      size_t i, 
      const ref& node,
      const bbox& bounds) {
      x.front()[i] = bounds.min[0]; y.front()[i] = bounds.min[1]; z.front()[i] = bounds.min[2];
      x.back ()[i] = bounds.max[0]; y.back ()[i] = bounds.max[1]; z.back ()[i] = bounds.max[2]; children[i] = node; }

    bool intersect(
      traversal& ray,
      const packet_float& tnear,
      const packet_float& tfar) const;

    bounds x, y, z;
    std::array<ref, M> children;
  };

  explicit tree(const std::vector<Object>& objects)
    : objects_{objects} {}

  bool optimize();
  auto& bounds() const { return bounds_; }

protected:
  ref  root_;
  bbox bounds_;

  std::vector<node> nodes_;
  std::vector<leaf> leaves_;
  std::vector<Object> objects_;
};

template <typename Object>
bool tree<Object>::optimize() {
  return true;
}

template <typename Object>
bool tree<Object>::node::intersect(traversal& ray, const packet_float& tnear, const packet_float& tfar) const {
  auto nx = (x[1 - ray.x_sgn] - ray.origin[0]) * ray.rcp_direction[0];
  auto ny = (y[1 - ray.y_sgn] - ray.origin[1]) * ray.rcp_direction[1];
  auto nz = (z[1 - ray.z_sgn] - ray.origin[2]) * ray.rcp_direction[2];
  auto fx = (x[    ray.x_sgn] - ray.origin[0]) * ray.rcp_direction[0];
  auto fy = (y[    ray.y_sgn] - ray.origin[1]) * ray.rcp_direction[1];
  auto fz = (z[    ray.z_sgn] - ray.origin[2]) * ray.rcp_direction[2];

  return 
    (ray.mask = movemask(
      (ray.distance = max(max(nx, ny), max(nz, tnear))) <= min(min(fx, fy), min(fz, tfar)))) == 0;
}

} // namespace ig

#endif // IG_MATH_TREE_H
