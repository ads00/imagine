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

#include "imagine/envi/impl_hw/image.h"
#include "imagine/envi/impl_hw/device.h"

namespace ig {

image::image(const device& device, VkImage image, image_format format)
  : VKObject<VkImage>{std::move(image)}
  , devi{device}
  , format_{format} {}

image::image(const device& device, type_t type, 
               image_format format, image_usages usages,
                 uint32_t width, uint32_t height, uint32_t depth, uint32_t layers, uint32_t mips) : image{device, 0, format} {

  VkImageCreateInfo image_info = {
    VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, // VkStructureType          sType;
    nullptr,                             // const void*              pNext;
    0,                                   // VkImageCreateFlags       flags;
    static_cast<VkImageType>(type),      // VkImageType              imageType;
    static_cast<VkFormat>(format),       // VkFormat                 format;
    {width, height, depth},              // VkExtent3D               extent;
    mips,                                // uint32_t                 mipLevels;
    layers,                              // uint32_t                 arrayLayers;
    VK_SAMPLE_COUNT_1_BIT,               // VkSampleCountFlagBits    samples;
    VK_IMAGE_TILING_OPTIMAL,             // VkImageTiling            tiling;
    usages,                              // VkImageUsageFlags        usage;
    VK_SHARING_MODE_EXCLUSIVE,           // VkSharingMode            sharingMode;
    0,                                   // uint32_t                 queueFamilyIndexCount;
    nullptr,                             // const uint32_t*          pQueueFamilyIndices;
    VK_IMAGE_LAYOUT_UNDEFINED            // VkImageLayout            initialLayout;
  };

  auto res = devi.vkCreateImage(devi, &image_info, nullptr, &h_);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create image : " + vulkan::to_string(res)};
  }
}

image::~image() {
  for (auto& view : views_)
    devi.vkDestroyImageView(devi, view.second, nullptr);

  if (memory_) {
    memory_->coalesce(block_);
    devi.vkDestroyImage(devi, h_, nullptr);
  }
}

auto image::view(view_type_t type, image_aspects aspects, 
                   uint32_t blayer, uint32_t layers, uint32_t bmip, uint32_t mips) -> iview& {
  auto view_it = views_.find(type);
  if (view_it == views_.end()) {
    VkImageViewCreateInfo imageview_info = {
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, // VkStructureType            sType;
      nullptr,                                  // const void*                pNext;
      0,                                        // VkImageViewCreateFlags     flags;
      h_,                                       // VkImage                    image;
      static_cast<VkImageViewType>(type),       // VkImageViewType            viewType;
      static_cast<VkFormat>(format_),           // VkFormat                   format;
      VkComponentMapping {                      // VkComponentMapping         components;
      VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
      VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY },
      VkImageSubresourceRange {                 // VkImageSubresourceRange    subresourceRange;
        aspects, // VkImageAspectFlags    aspectMask;
        bmip,    // uint32_t              baseMipLevel;
        mips,    // uint32_t              levelCount;
        blayer,  // uint32_t              baseArrayLayer;
        layers } // uint32_t              layerCount;                             
    };

    auto res = devi.vkCreateImageView(devi, &imageview_info, nullptr, &views_[type]);
    if (res != VK_SUCCESS) {
      throw std::runtime_error{"Failed to create imageview : " + vulkan::to_string(res)};
    } 
    return views_[type];
  } else {
    return view_it->second;
  }
}

void image::bind(memory& memory, memory::block_iterator block, uint64_t aligned) {
  assert(devi.vkBindImageMemory(devi, h_, memory, aligned) == VK_SUCCESS 
         && "Failed to bind image to memory");
  memory_ = &memory, block_ = block, aligned_ = aligned;
}

auto image::requirements(uint64_t& size, uint64_t& alignment) -> uint32_t {
  VkMemoryRequirements 
    mem_reqs{};
  devi.vkGetImageMemoryRequirements(devi, h_, &mem_reqs);

  size = mem_reqs.size, alignment = mem_reqs.alignment;
  return mem_reqs.memoryTypeBits;
}

} // namespace ig
