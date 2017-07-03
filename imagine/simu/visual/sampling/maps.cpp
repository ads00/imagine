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

#include "imagine/simu/visual/sampling/maps.h"

namespace ig     {
namespace sample {

vec3 hemisphere_uniform(const vec2& s) {
  assert(s[0] >= 0 && s[0] < 1 && "Invalid mapping sample");
  assert(s[1] >= 0 && s[1] < 1 && "Invalid mapping sample");

  auto cos_theta = 1 - s[1];
  auto sin_theta = std::sqrt(1 - cos_theta * cos_theta);
  return spherical(two_pi<float> * s[0], cos_theta, sin_theta);
}

vec3 hemisphere_cosine(const vec2& s) {
  assert(s[0] >= 0 && s[0] < 1 && "Invalid mapping sample");
  assert(s[1] >= 0 && s[1] < 1 && "Invalid mapping sample");

  return spherical(two_pi<float> * s[0], std::sqrt(1 - s[1]), std::sqrt(s[1]));
}

vec3 cone_uniform(const vec2& s, float cos_theta_max) {
  assert(s[0] >= 0 && s[0] < 1 && "Invalid mapping sample");
  assert(s[1] >= 0 && s[1] < 1 && "Invalid mapping sample");

  auto cos_theta = (1 - s[1]) + s[1] * cos_theta_max;
  auto sin_theta = std::sqrt(1 - cos_theta * cos_theta);
  return spherical(two_pi<float> * s[0], cos_theta, sin_theta);
}

vec3 sphere_uniform(const vec2& s) {
  assert(s[0] >= 0 && s[0] < 1 && "Invalid mapping sample");
  assert(s[1] >= 0 && s[1] < 1 && "Invalid mapping sample");

  auto cos_theta = 1 - 2 * s[1];
  auto sin_theta = std::sqrt(1 - cos_theta * cos_theta);
  return spherical(two_pi<float> * s[0], cos_theta, sin_theta);
}

vec3 triangle_uniform(const vec2& s) {
  assert(s[0] >= 0 && s[0] < 1 && "Invalid mapping sample");
  assert(s[1] >= 0 && s[1] < 1 && "Invalid mapping sample");

  auto sq0 = std::sqrt(s[0]);
  return vec3 { 1 - sq0, (1 - s[1]) * sq0, s[1] * sq0 };
}

vec2 disk_uniform(const vec2& s) {
  assert(s[0] >= 0 && s[0] < 1 && "Invalid mapping sample");
  assert(s[1] >= 0 && s[1] < 1 && "Invalid mapping sample");

  auto a = 2 * s[0] - 1, b = 2 * s[1] - 1, r = std::max(a * a, b * b);
  auto phi = a * a > b * b
    ? pi<float> / 4 * (b / a)
    : b != 0
      ? pi<float> / 2 - pi<float> / 4 * (a / b)
      : 0;
  return vec2 { r * std::cos(phi), r * std::sin(phi) };
}

float disk_gaussian_area_pdf(float r, float a) { return a * pi<float> / 4 * std::exp(-a * r * r); }
float disk_gaussian(float s, float a) {
  assert(s >= 0 && s < 1 && "Invalid mapping sample");
  return std::sqrt(-std::log(1 - s) / a);
}

vec2 circle_uniform(float s) {
  assert(s >= 0 && s < 1 && "Invalid mapping sample");
  auto phi = two_pi<float> * s; return vec2 { std::cos(phi), std::sin(phi) };
}

float exponential_segment_pdf(float x, float a, float l, float r) { return a / (std::exp(a * (x - l)) - std::exp(a * (x - r))); }
float exponential_segment(float s, float a, float l, float r) {
  assert(s >= 0 && s < 1 && "Invalid mapping sample");
  return std::min(std::max(l - std::log(1 - s * (1 - std::exp(-(r - l) * a))) / a, l), r);
}

float equiangular_pdf(float t, float theta_a, float theta_b, float d) { return d / ((theta_b - theta_a) * (d * d + t * t)); }
float equiangular(float s, float theta_a, float theta_b, float d) {
  assert(s >= 0 && s < 1 && "Invalid mapping sample");
  return d * std::tan((1 - s) * theta_a + s * theta_b);
}

} // namespace sample
} // namespace ig
