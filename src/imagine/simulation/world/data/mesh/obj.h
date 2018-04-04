/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_SIMULATION_OBJ_H
#define IG_SIMULATION_OBJ_H

#include "imagine/simulation/world/data/mesh/base_mesh.h"
#include <sstream>

namespace ig     {
namespace detail {

using obj   = mesh_bridge::data;
using obj_t = mesh_bridge::resource;
// obj wavefront
// supports vertices with positions, normals and texture coordinates without materials
// extensions -> .obj
// validate -> none
// standard -> see Wavefront Technologies

bool obj_validate(std::istream& stream);
obj  obj_readp_impl(std::istream& stream, const mesh_bridge::parameters&);
bool obj_write_impl(std::ostream& stream, const mesh_bridge::parameters&, const mesh_bridge::resource& mesh);

} // namespace detail
} // namespace ig

#endif // IG_SIMULATION_OBJ_H
