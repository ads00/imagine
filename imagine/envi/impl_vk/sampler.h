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

#ifndef IG_ENVI_VK_SAMPLER_H
#define IG_ENVI_VK_SAMPLER_H

#include "imagine/envi/impl_vk/image.h"

namespace ig {
namespace vk {

enum class filter;
enum class mipmap_mode;
enum class address_mode;

class ig_api image::sampler : public managed<VkSampler_T*> {
public:
  explicit sampler(const device& device, filter mag, filter min, address_mode mode, mipmap_mode mipmap, float anisotropy = 0.f);
  virtual ~sampler();

  const device& devi; 
};

enum class filter      { nearest = 0, linear = 1, cubic = 1000015000 };
enum class mipmap_mode { nearest = 0, linear = 1 };

enum class address_mode {
  repeat     = 0, mirrored_repeat = 1, 
  clamp_edge = 2, clamp_border    = 3, mirror_clamp_edge = 4 };

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_SAMPLER_H
