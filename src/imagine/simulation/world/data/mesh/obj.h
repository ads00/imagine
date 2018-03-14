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
