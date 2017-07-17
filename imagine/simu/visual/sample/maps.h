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

#ifndef IG_SIMU_MAPS_H
#define IG_SIMU_MAPS_H

#include "imagine/math/geom/proj.h"

namespace ig     {
namespace sample {

template <typename T>
auto spherical(T phi, T cos_theta, T sin_theta) 
{ return vec3{std::cos(phi) * sin_theta, cos_theta, std::sin(phi) * sin_theta}; }

template <typename T>
auto cylindrical(T theta, T y)
{ return vec3{std::cos(theta), y, std::sin(theta)}; }

ig_api vec3 hemisphere_uniform(const vec2& s);
ig_api vec3 hemisphere_cosine (const vec2& s);
ig_api vec3 cone_uniform      (const vec2& s, float cos_theta_max);
ig_api vec3 sphere_uniform    (const vec2& s);
ig_api vec3 triangle_uniform  (const vec2& s);

ig_api vec2 disk_uniform           (const vec2& s);
ig_api float disk_gaussian_area_pdf(float r, float a);
ig_api float disk_gaussian         (float s, float a);
ig_api vec2 circle_uniform         (float s);

ig_api float exponential_segment_pdf(float x, float a, float l, float r);
ig_api float exponential_segment(float s, float a, float l, float r);

ig_api float equiangular_pdf(float t, float theta_a, float theta_b, float d);
ig_api float equiangular    (float s, float theta_a, float theta_b, float d);

} // namespace sample
} // namespace ig

#endif // IG_SIMU_MAPS_H
