/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_TRAVERSAL_H
#define IG_MATH_TRAVERSAL_H

#include "imagine/math/geom/proj.h"
#include "imagine/math/geom/spatial/ray.h"

namespace ig {

struct traversal {
  static constexpr size_t depth = 32;
  static constexpr size_t stack_size = IG_PACKET_WIDE * depth;

  explicit traversal(const vec3& origin, const vec3& direction)
    : origin{origin}
    , rcp_direction{1 / direction}
    , x_sgn{direction[0] >= 0}
    , y_sgn{direction[1] >= 0}
    , z_sgn{direction[2] >= 0} {}

  template <typename Traverse, typename Node> auto closest(Traverse& stack, const Node& node);
  template <typename Traverse, typename Node> auto any    (Traverse& stack, const Node& node);

  template <typename Hierarchy, typename Walk, typename Visitor>
  auto traverse(
    const Hierarchy& root,
    Walk&& walk,
    Visitor&& visitor,
    ray& ray);

  pvec3 origin, rcp_direction;
  packet distance;
  size_t mask;
  uint32_t x_sgn,
           y_sgn,
           z_sgn;
};

template <typename Traverse, typename Node>
auto traversal::closest(Traverse& stack, const Node& node) {
  auto r = ctz_f(mask);
  auto s = stack;
  if (mask == 0) return node.children[r];

  auto c0 = node.children[r];
  auto d0 = static_cast<uint32_t>(distance[r]);
  r = ctz_f(mask);
  auto c1 = node.children[r];
  auto d1 = static_cast<uint32_t>(distance[r]);
  if (mask == 0) {
    if (d0 < d1) {
      stack->item = c1;
      stack->dist = d1;
      stack++;
      return c0;
    } else {
      stack->item = c0;
      stack->dist = d0;
      stack++;
      return c1;
    }
  }

  stack->item = c0;
  stack->dist = d0; stack++;
  stack->item = c1;
  stack->dist = d1; stack++;

  for (;;) {
    r = ctz_f(mask);
    stack->item = node.children[r];
    stack->dist = static_cast<uint32_t>(distance[r]);
    stack++;
    if (mask == 0) {
      std::sort(
        s,
        stack,
        [](auto& lhs, auto& rhs) { return lhs.dist > rhs.dist; });
      return stack--[-1].item;
    }
  }
}

template <typename Traverse, typename Node>
auto traversal::any(Traverse& stack, const Node& node) {
  auto r = ctz_f(mask);
  if (mask == 0) return node.children[r];
  for (;;) {
    stack->item = node.children[r];
    stack++;
    r = ctz_f(mask);
    if (mask == 0) return node.children[r];
  }
}

template <typename Hierarchy, typename Walk, typename Visitor>
auto traversal::traverse(const Hierarchy& root, Walk&& walk, Visitor&& visitor, ray& ray) {
  struct stack_item { typename Hierarchy::ref item; size_t dist; };

  std::array
  < stack_item,
    stack_size > stack; stack[0].item = root.root_; stack[0].dist = 0;
  auto stack_ptr = std::next(stack.begin());

  bool hit = false;
  while (stack_ptr != stack.begin()) {
    --stack_ptr;
    auto current = stack_ptr->item;

    for (;;) {
      if (current.is_leaf()) {
        auto proceed = visitor(hit, current);
        if (proceed) return true;
        else
        break;
      } else {
        auto node = root.nodes_[current.index];
        if (node.intersect(*this, ray.tnear, ray.tfar)) break;

        current = walk(
          stack_ptr,
          node);
      }
    }
  } return hit;
}

} // namespace ig

#endif // IG_MATH_TRAVERSAL_H
