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

#include "imagine/envi/impl_vk/sampler.h"
#include "imagine/envi/impl_vk/detail/vulkan.h"

namespace ig {
namespace vk {

image::sampler::sampler(const device& device, filter mag, filter min, address_mode mode, mipmap_mode mipmap, float anisotropy)
  : devi{device} {

  VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.flags = 0;
    sampler_info.magFilter = static_cast<VkFilter>(mag);
    sampler_info.minFilter = static_cast<VkFilter>(min);
    sampler_info.addressModeU =
    sampler_info.addressModeV =
    sampler_info.addressModeW = static_cast<VkSamplerAddressMode>(mode);
    sampler_info.mipmapMode   = static_cast<VkSamplerMipmapMode>(mipmap);
      sampler_info.maxAnisotropy    = anisotropy;
      sampler_info.anisotropyEnable = anisotropy > 0;
      sampler_info.minLod = 0.0f;
      sampler_info.maxLod = 0.25f;

  auto res = devi->vkCreateSampler(devi, &sampler_info, nullptr, &handle);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{ "Failed to create sampler : " + vulkan::to_string(res) };
  }
}

image::sampler::~sampler() {
  devi->vkDestroySampler(devi, handle, nullptr);
}

} // namespace vk
} // namespace ig
