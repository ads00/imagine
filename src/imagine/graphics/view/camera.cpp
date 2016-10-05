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

#include "imagine/graphics/view/camera.h"

namespace ig {

camera::camera(projection_t projection, size_t w, size_t h)
  : camera{projection, w, h, vec3{0.f}, vec3{0.f}, vec3{0.f, 1.f, 0.f}} {}

camera::camera(projection_t projection, size_t w, size_t h, const vec3& pos, const vec3& target, const vec3& up)
  : projection_{projection}
  , w_{w}
  , h_{h}
  , pos_{pos}
  , target_{target}
  , up_{up}
  , zn_{1e-3f}, zf_{1e3f}, fovy_{1.57f}
  , uview_{false}, uproj_{false} {}

void camera::make_orthographic() {
  uproj_ = false;
  projection_ = projection_t::orthographic;
}

void camera::make_perspective(float fovy) {
  uproj_ = false;
  fovy_ = fovy;
  projection_ = projection_t::perspective;
}

void camera::clip(float zn, float zf) {
  uproj_ = false;
  zn_ = zn, zf_ = zf;
}

ray3 camera::cast_ray(size_t x, size_t y) const {
  auto nx = x / static_cast<float>(w_) * 2.f - 1.f;
  auto ny = y / static_cast<float>(h_) * 2.f - 1.f;

  auto raster = iproj_.transform(vec3{nx, ny, 0.f});
  vec3 wo{}, wd{};
  if (projection_ == projection_t::perspective) {
    wo = iview_.transform(vec3{0.f});
    wd = iview_.transform(raster, true);
  } else {
    wo = iview_.transform(raster);
    wd = iview_.transform(vec3{0.f, 0.f, -1.f}, true);
  }
  return ray3{wo, linalg::normalise(wd)};
}

const mat4& camera::view() {
  if (!uview_) {
    view_ = mat4::look(pos_, target_, up_), iview_ = linalg::inv(view_);
    uview_ = true;
  }
  return view_;
}

const mat4& camera::proj() {
  if (!uproj_) {
    switch (projection_) {
    case projection_t::orthographic:
      proj_ = mat4::orthographic(1, 1, zn_, zf_);
      break;
    case projection_t::perspective:
      proj_ = mat4::perspective(fovy_, static_cast<float>(w_ / h_), zn_, zf_);
      break;
    }
    iproj_ = linalg::inv(proj_);
    uproj_ = true;
  }
  return proj_;
}

} // namespace ig
