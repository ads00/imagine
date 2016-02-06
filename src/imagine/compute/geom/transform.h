/*
 Copyright (c) 2015, 2016
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

#ifndef COMPUTE_TRANSFORM_H
#define COMPUTE_TRANSFORM_H

#include "imagine/compute/geom/homogeneous.h"

namespace ig
{

class IG_API transform
{
public:
  enum space_t { local, world };

  constexpr transform(const vec3& pos, const quat& ori, const vec3& sca);
  ~transform();

  void positions(const vec3& pos, space_t space = space_t::local);
  void directs(const quat& ori, space_t space = space_t::local);
  void scales(const vec3& sca, space_t space = space_t::local);

  transform& translate(const vec3& tra, space_t space = space_t::local);
  transform& rotate(const quat& rot, space_t space = space_t::local);
  transform& scale(const vec3& sca);

  void link(transform* parent);

  const mat4& world_transform();
  const mat4  inv_wt();

private:
  void remove_child(const transform& tr);
  void needs_update();

  transform* parent_;
  std::vector< std::reference_wrapper<transform> > children_;

  bool umat_;
  mat4 matrix_;
  vec3 pos_; quat ori_; vec3 sca_;
};

} // namespace ig

#endif // COMPUTE_TRANSFORM_H
