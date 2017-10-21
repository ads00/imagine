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

#include "imagine/envi/impl_vk/image.h"
#include "imagine/envi/impl_vk/detail/vulkan.h"

namespace ig {
namespace vk {

image::image(const device& device, VkImage_T* image, format format)
  : managed<VkImage_T*>{image}
  , resource{device}
  , format_{format} {}

image::image(
  const device& device, 
  type_t type,
  format format, 
  image_usages usages,
  uint32_t width, uint32_t height, uint32_t depth, uint32_t layers, uint32_t mips) : image{device, 0, format} {

  VkImageCreateInfo image_info {};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.flags = 0;
    image_info.imageType = static_cast<VkImageType>(type);
    image_info.format    = static_cast<VkFormat>(format);
    image_info.extent      = {width, height, depth};
    image_info.mipLevels   = mips;
    image_info.arrayLayers = layers;
    image_info.usage       = usages;
      image_info.samples = VK_SAMPLE_COUNT_1_BIT;
      image_info.tiling  = VK_IMAGE_TILING_OPTIMAL;

  auto res = 
    devi->vkCreateImage(
      devi, 
      &image_info, 
      nullptr, 
      &handle);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create image : " + vulkan::to_string(res)};
  }
}

image::~image() {
  for (auto& view : views_)
    devi->vkDestroyImageView(
      devi, 
      view.second, 
      nullptr);
  if (block_)
    devi->vkDestroyImage(
      devi, 
      handle, 
      nullptr);
}

auto image::view(view_type_t type, image_aspects asp, uint32_t blayer, uint32_t layers, uint32_t bmip, uint32_t mips) -> iview& {
  auto view_it = views_.try_emplace(type);
  if (view_it.second) {
    VkImageViewCreateInfo imageview_info {};
      imageview_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      imageview_info.flags = 0;
      imageview_info.viewType = static_cast<VkImageViewType>(type);
      imageview_info.format   = static_cast<VkFormat>(format_);
      imageview_info.image            = handle;
      imageview_info.subresourceRange = {asp, bmip, mips, blayer, layers};
        imageview_info.components = {
        VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };

    auto res = 
      devi->vkCreateImageView(
        devi, 
        &imageview_info, 
        nullptr, 
        &view_it.first->second);
    if (res != VK_SUCCESS) {
      throw std::runtime_error{"Failed to create image view : " + vulkan::to_string(res)};
    } 
  } return view_it.first->second;
}

bool image::bind(block_ptr block) {
  block_ = std::move(block);
  return devi->vkBindImageMemory(
    devi, 
    handle, 
    block_->mem, 
    block_->offset) == VK_SUCCESS;
}

auto image::require(uint64_t& size, uint64_t& alignment) -> uint32_t {
  VkMemoryRequirements mem_reqs{};
  devi->vkGetImageMemoryRequirements(
    devi, 
    handle, 
    &mem_reqs);

  size = mem_reqs.size, alignment = mem_reqs.alignment;
  return mem_reqs.memoryTypeBits;
}

} // namespace vk
} // namespace ig
