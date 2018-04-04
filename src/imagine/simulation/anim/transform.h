/*
 Imagine v0.1
 [simulation]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_SIMULATION_TRANSFORM_H
#define IG_SIMULATION_TRANSFORM_H

#include "imagine/math/geometry/proj.h"

namespace ig {

class IG_API transform {
public:
  transform() = default;
  explicit transform(const mat4& trf);
  explicit transform(const vec3& pos, const quat& ori, const vec3& sca);
  virtual ~transform();

  void positions(const vec3& pos, coordinate coord = coordinate::local);
  void directs(const quat& ori, coordinate coord = coordinate::local);
  void scales(const vec3& sca, coordinate coord = coordinate::local);

  transform& translate(const vec3& tra, coordinate coord = coordinate::local);
  transform& rotate(const quat& rot, coordinate coord = coordinate::local);
  transform& scale(const vec3& sca);

  void link(transform* parent);

  const mat4& object_world();
  const mat4& world_object();

private:
  void remove_child(const transform& tr);
  transform& hierarchical_invalidate();

private:
  transform* parent_;
  std::vector<transform*> children_;

  bool uow_, uwo_;
  mat4  ow_,  wo_;
  vec3 pos_; quat ori_; vec3 sca_;
};

} // namespace ig

#endif // IG_SIMULATION_TRANSFORM_H
