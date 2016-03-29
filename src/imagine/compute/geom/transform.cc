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

#include "imagine/compute/geom/transform.h"
#include "imagine/compute/linalg/geometric.h"

namespace ig
{

constexpr transform::transform(const vec3& pos, const quat& ori, const vec3& sca)
  : parent_{nullptr}, umat_{false}, matrix_{mat4::eye}, pos_{pos}, ori_{ori}, sca_{sca}
{
}

transform::~transform()
{
  if (parent_)
  {
    for (auto& child : children_) child.get().parent_ = nullptr;
    parent_->remove_child(*this);
  }
}

void transform::positions(const vec3& pos, space_t space)
{
  switch (space)
  {
  case space_t::local:
    pos_ = pos;
    break;

  case space_t::world:
    pos_ = parent_ ? parent_->inv_wt().transform(pos) : pos;
    break;
  }
}

void transform::directs(const quat& ori, space_t space)
{
  switch (space)
  {
  case space_t::local:
    ori_ = ori;
    break;

  case space_t::world:
    ori_ = parent_ ? parent_->ori_ * ori_ : ori_;
    break;
  }
}

void transform::scales(const vec3& sca, space_t space)
{
  switch (space)
  {
  case space_t::local:
    sca_ = sca;
    break;

  case space_t::world:
    sca_ = parent_ ? sca / parent_->sca_ : sca_;
    break;
  }
}

transform& transform::translate(const vec3& tra, space_t space)
{
  switch (space)
  {
  case space_t::local:
    pos_ += linalg::rotate(ori_, tra);
    break;

  case space_t::world:
    pos_ += 
      parent_ ? parent_->inv_wt().transform(tra) :
      tra;
    break;
  }

  needs_update();
  return *this;
}

transform& transform::rotate(const quat& rot, space_t space)
{
  switch (space)
  {
  case space_t::local:
    ori_ = linalg::normalise(ori_ * rot);
    break;

  case space_t::world:
    const auto into = 
      parent_ ? parent_->ori_ * ori_ : 
      ori_;

    ori_ = 
      parent_ ? ori_ * linalg::inv(into) * rot * into :
      linalg::normalise(rot * into);
    break;
  }

  needs_update();
  return *this;
}

transform& transform::scale(const vec3& sca)
{
  sca_ %= sca; needs_update();
  return *this;
}

void transform::remove_child(const transform& tr)
{
  children_.erase(std::remove_if(children_.begin(), children_.end(), [this, &tr](const auto& c)
  {
    return &c.get() == &tr;
  }), children_.end());
}

void transform::link(transform* parent)
{
  assert(parent != this && "Parent transform can't be itself");

  if (parent_) parent_->remove_child(*this);

  parent_ = parent;
  needs_update();

  if (parent_) parent_->children_.push_back(*this);
}

const mat4& transform::wt()
{
  if (!umat_)
  {
    matrix_ = mat4::translating(pos_) * mat4::rotating(ori_) * mat4::scaling(sca_);
    umat_ = true;

    if (parent_) matrix_ = parent_->wt() * matrix_;
  }

  return matrix_;
}

const mat4 transform::inv_wt()
{
  return linalg::inv(wt());
}

void transform::needs_update()
{
  if (!umat_) return;
  else
  {
    umat_ = false;
    for (auto& child : children_) child.get().needs_update();
  }
}

} // namespace ig
