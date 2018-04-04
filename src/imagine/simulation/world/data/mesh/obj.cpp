/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/simulation/world/data/mesh/obj.h"
#include "imagine/core/log.h"

#include <cctype>

namespace ig     {
namespace detail {

struct obj_src : public base_mesh {
  template <typename T>
  auto vertex(int32_t id, const std::vector<T>& components) {
    return id > 0
      ? components[id - 1]
      : id != 0
        ? components[id + static_cast<int32_t>(components.size())]
        : T{};
  }

  std::vector<vec3> p;
  std::vector<vec2> t;
  std::vector<vec3> n;
};

// Obj interface implementation - validate - read - write
bool obj_validate(std::istream& stream) {
  return true;
}

void parse_vertex(obj_src& src, std::string_view id, std::string_view line) {
  if (id == "v") {
    vec3 p;
    std::sscanf(
      line.data(), "%f %f %f",
      &p[0],
      &p[1],
      &p[2]); src.p.emplace_back(p);
  } else if (id == "vn") {
    vec3 n;
    std::sscanf(
      line.data(), "%f %f %f",
      &n[0],
      &n[1],
      &n[2]); src.n.emplace_back(n);
  } else if (id == "vt") {
    vec2 t;
    std::sscanf(
      line.data(), "%f %f",
      &t[0],
      &t[1]); src.t.emplace_back(t);
  }
}

void parse_face(obj_src::pattern* pattern, std::string_view line, size_t face_count) {
  size_t offset = 0;
  for (uint32_t i = 0; i < face_count; ++i) {
    offset = line.find_first_of(' ', offset + 1);
    obj_src::vert v{};
    if (std::sscanf(&line[offset], "%d/%d/%d", &v.p, &v.t, &v.n) == 3 ||
        std::sscanf(&line[offset], "%d//%d"  , &v.p,       &v.n) == 2 ||
        std::sscanf(&line[offset], "%d/%d"   , &v.p, &v.t      ) == 2 ||
        std::sscanf(&line[offset], "%d"      , &v.p            ) == 1) {
      pattern->verts.push_back(v);
    } else {
      log_(
        info,
        "[obj_loader] Failed to load face, unrecognized line {}",
        line);
      return;
    }
  }
}

obj obj_readp_impl(std::istream& stream, const mesh_bridge::parameters&) {
  obj_src src;
  obj_src::pattern* curr = &src.add_pattern();

  std::string line;
  while (std::getline(stream, line)) {
    if (line.empty()) continue;
    else {
      switch (std::tolower(line[0])) {
        case 'g':
        case 'o': curr = &src.add_pattern();
        break;
        case 'v':
        {
          auto id = line.find(' ');
          parse_vertex(src, line.substr(0, id), line.substr(id));
        }
        break;
        case 'f':
        {
          obj_src::face
          face{};
          face.index = curr->verts.size();
          face.count = std::count_if(
            line.begin(),
            line.end(),
            [](char c) { return c == ' '; });

          curr->faces.push_back(face); parse_face(curr, line, face.count);
        }
        break;
        default: {}
      }
    }
  }

  mesh_bridge::exchange meshes;
  std::transform(
    src.patterns.begin(),
    src.patterns.end(),
    std::back_inserter(meshes),
    [&src](auto& pattern) {
      return src.generate_mesh(
        pattern,
        [&src](auto& mesh, auto& v) {
          mesh->vertices.push_back({src.vertex(v.p, src.p), src.vertex(v.n, src.n), src.vertex(v.t, src.t)});
        });
    });

  return std::make_pair(true, std::move(meshes));
}

bool obj_write_impl(std::ostream& stream, const mesh_bridge::parameters&, const mesh_bridge::resource& mesh) {
  return true;
}

} // namespace detail
} // namespace ig
