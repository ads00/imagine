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

#include "imagine/envi/_vk/buffer.h"
#include "imagine/envi/_vk/detail/vulkan.h"

namespace ig {
namespace vk {

buffer::buffer(const device& device, uint64_t size, buffer_usages usages)
  : resource{device} {

  VkBufferCreateInfo buffer_info {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.flags = 0;
    buffer_info.size  = size;
    buffer_info.usage = usages;

  auto res = devi->vkCreateBuffer(devi, &buffer_info, nullptr, &handle);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create buffer : " + vulkan::to_string(res)};
  }
}

buffer::~buffer() {
  for (auto& view : views_)
    devi->vkDestroyBufferView(devi, view.second, nullptr);
  if (block_)
    devi->vkDestroyBuffer(devi, handle, nullptr);
}

auto buffer::view(format format, uint64_t offset, uint64_t range ) -> bview& {
  auto view_it = views_.try_emplace(format);
  if (view_it.second) {
    VkBufferViewCreateInfo bufferview_info {};
      bufferview_info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
      bufferview_info.flags = 0;
      bufferview_info.format = static_cast<VkFormat>(format);
      bufferview_info.buffer = handle;
      bufferview_info.offset = offset;
      bufferview_info.range  = range;

    auto res = devi->vkCreateBufferView(devi, &bufferview_info, nullptr, &view_it.first->second);
    if (res != VK_SUCCESS) {
      throw std::runtime_error{"Failed to create buffer view : " + vulkan::to_string(res)};
    }
  }
  return view_it.first->second;
}

bool buffer::bind(std::unique_ptr<block_type> block) {
  block_ = std::move(block);
  return devi->vkBindBufferMemory(devi, handle, block_->mem, block_->offset) == VK_SUCCESS;
}

auto buffer::require(uint64_t& size, uint64_t& alignment) -> uint32_t {
  VkMemoryRequirements 
    mem_reqs{};
  devi->vkGetBufferMemoryRequirements(devi, handle, &mem_reqs);

  size = mem_reqs.size, alignment = mem_reqs.alignment;
  return mem_reqs.memoryTypeBits;
}

} // namespace vk
} // namespace ig
