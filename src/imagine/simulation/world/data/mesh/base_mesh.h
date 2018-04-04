/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_SIMULATION_BASE_MESH_H
#define IG_SIMULATION_BASE_MESH_H

#include "imagine/simulation/world/data/bridge.h"
#include <unordered_map>

namespace ig {

struct base_mesh {
  struct vert { int32_t p, t, n; };
  struct face { size_t index, count; };
  struct pattern { std::vector<face> faces; std::vector<vert> verts; };

  base_mesh()
    : current (0)
    , patterns(1) {}

  auto add_pattern() -> pattern&;
  template <typename VertexGen> auto generate_mesh(const pattern& src, VertexGen&& vg) const;

  uint32_t current;
  std::vector<pattern> patterns;
};

inline auto base_mesh::add_pattern() -> pattern& {
  return current++ <= patterns.size()
    ? patterns.back()
    : patterns.emplace_back();
}

template <typename VertexGen>
auto base_mesh::generate_mesh(const pattern& src, VertexGen&& vg) const {
  auto hash = [](auto& v) { return hash_combine(hash_combine(hash_combine(0, v.p), v.t), v.n); };
  auto cmpt = [](auto& lhs, auto& rhs) {
    return lhs.p == rhs.p &&
           lhs.t == rhs.t &&
           lhs.n == rhs.n;
  };

  std::unordered_map<vert, size_t, decltype(hash), decltype(cmpt)> vertices{
    src.verts.size(),
    hash,
    cmpt};

  auto mesh = std::make_unique<mesh_s>();
  for (auto& face : src.faces) {
    std::vector<uint32_t> face_ids(face.count);
    for (size_t i = 0; i < face_ids.size(); ++i) {
      auto [vertex, ins] = vertices.try_emplace(src.verts[face.index + i], vertices.size());
      if (ins)
        vg(mesh,
           vertex->first);
      face_ids[i] = vertex->second;
    }

    for (size_t i = 1; i < face.count - 1; ++i) {
      mesh->faces.push_back({
        face_ids[0],
        face_ids[i],
        face_ids[i + 1]});
    }
  } return mesh;
}

} // namespace ig

#endif // IG_SIMULATION_BASE_MESH_H
