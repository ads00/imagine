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

#include "imagine/simu/visual/camera.h"

namespace ig {

camera::camera(planar_proj proj, size_t w, size_t h)
  : camera{proj, w, h, vec3{0.f}, vec3{0.f}, vec3{0.f, 1.f, 0.f}} {}

camera::camera(planar_proj proj, size_t w, size_t h, const vec3& pos, const vec3& target, const vec3& up)
  : projection_{proj}
  , w_{w}
  , h_{h}
  , pos_{pos}
  , target_{target}
  , up_{up}
  , zn_{1e-3f}, zf_{1e3f}, fovy_{radians(70.f)}
  , unit_{true}, uview_{false}, uproj_{false} {}

void camera::extent(float fovy) {
  uproj_ = false;
  fovy_ = fovy;
}

void camera::clip(float zn, float zf) {
  uproj_ = false;
  zn_ = zn, zf_ = zf;
}

ray3 camera::cast_ray(size_t x, size_t y) const {
  auto nx = x / static_cast<float>(w_) * 2.f - 1.f; 
  auto ny = y / static_cast<float>(h_) * 2.f - 1.f;

  auto raster = trf::transform(iproj_, vec3{nx, ny, 1.f}, unit_);
  vec3 
    wo{}, 
    wd{};
  if (projection_ == planar_proj::perspective) {
    wo = pos_;
    wd = trf::transform(iview_, raster, true);
  } else {
    wo = trf::transform(iview_, raster, true);
    wd = -pos_;
  }
  return ray3{wo, lin::normalise(wd)};
}

const mat4& camera::view() {
  if (!uview_) {
    view_ = trf::look(pos_, target_, up_), iview_ = lin::inv(view_);
    uview_ = true;
  } return view_;
}

const mat4& camera::proj() {
  if (!uproj_) {
    switch (projection_) {
    case planar_proj::perspective:
      proj_ = trf::perspective(fovy_, static_cast<float>(w_) / h_, zn_, zf_);
      unit_ = false;
      break;
    case planar_proj::orthographic:
      proj_ = trf::orthographic(0, static_cast<float>(w_), 0, static_cast<float>(h_), zn_, zf_);
      unit_ = true;
      break;
    }
    iproj_ = lin::inv(proj_);
    uproj_ = true;
  } return proj_;
}

} // namespace ig
