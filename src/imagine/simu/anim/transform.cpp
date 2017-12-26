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

#include "imagine/simu/anim/transform.h"

namespace ig {

transform::transform(const mat4& trf)
  : parent_{nullptr}
  , uow_{true}
  , wo_{trf} {}

transform::transform(const vec3& pos, const quat& ori, const vec3& sca)
  : parent_{nullptr}
  , uow_{false}
  , uwo_{false}
  , ow_{mat4::eye()}
  , pos_{pos}
  , ori_{ori}
  , sca_{sca} {}

transform::~transform() {
  for (auto& child : children_)
    child->parent_ = nullptr;
  if (parent_) parent_->remove_child(*this);
}

void transform::positions(const vec3& pos, coordinate coord) {
  switch (coord) {
    case coordinate::local:
      pos_ = pos;
      break;
    case coordinate::world:
      pos_ = parent_
        ? trf::transform(parent_->world_object(), pos)
        : pos;
      break;
    default: {}
  }
}

void transform::directs(const quat& ori, coordinate coord) {
  switch (coord) {
    case coordinate::local:
      ori_ = ori;
      break;
    case coordinate::world:
      ori_ = parent_
        ? parent_->ori_ * ori_
        : ori_;
      break;
    default: {}
  }
}

void transform::scales(const vec3& sca, coordinate coord) {
  switch (coord) {
    case coordinate::local:
      sca_ = sca;
      break;
    case coordinate::world:
      sca_ = parent_
        ? sca / parent_->sca_
        : sca_;
      break;
    default: {}
  }
}

transform& transform::translate(const vec3& tra, coordinate coord) {
  switch (coord) {
    case coordinate::local:
      pos_ += spt::rotate(ori_, tra);
      break;
    case coordinate::world:
      pos_ += parent_
        ? trf::transform(parent_->world_object(), tra)
        : tra;
      break;
  } return hierarchical_invalidate();
}

transform& transform::rotate(const quat& rot, coordinate coord) {
  switch (coord) {
    case coordinate::local:
      ori_ = spt::normalise(ori_ * rot);
      break;
    case coordinate::world:
      auto into = parent_
        ? parent_->ori_ * ori_
        : ori_;

      ori_ = parent_
        ? ori_ * spt::inv(into) * rot * into
        : spt::normalise(rot * into);
      break;
  } return hierarchical_invalidate();
}

transform& transform::scale(const vec3& sca) {
  sca_ *= sca;
  return hierarchical_invalidate();
}

void transform::remove_child(const transform& tr) {
  children_.erase(
    std::remove(
      children_.begin(),
      children_.end(),
      &tr),
    children_.end());
}

void transform::link(transform* parent) {
  assert(parent != this && "Reflexive transform classes are not allowed");

  if (parent_ == parent) return;
  if (parent_) parent_->remove_child(*this);

  parent_ = parent;
  if (parent_) {
    parent_->children_.emplace_back(this);
  } hierarchical_invalidate();
}

const mat4& transform::object_world() {
  if (!uow_) {
    auto trs = trf::translation(pos_) * trf::rotation(ori_) * trf::scale(sca_);
    if (parent_) {
      ow_ = parent_->object_world() * trs;
    } else {
      ow_ = trs;
    } uow_ = true;
  } return ow_;
}

const mat4& transform::world_object() {
  if (!uwo_) {
    wo_ = lin::inv(object_world());
    uwo_ = true;
  } return ow_;
}

transform& transform::hierarchical_invalidate() {
  if (uow_) {
    uow_ = false;
    uwo_ = false;
    for (auto& child : children_) child->hierarchical_invalidate();
  } return *this;
}

} // namespace ig
