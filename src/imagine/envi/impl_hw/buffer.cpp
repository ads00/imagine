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

#include "imagine/envi/impl_hw/buffer.h"
#include "imagine/envi/impl_hw/device.h"

namespace ig {

buffer::buffer(const device& device, uint64_t size, buffer_usages usages)
  : devi{device} {

  VkBufferCreateInfo buffer_info = {
    VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, // VkStructureType        sType;
    nullptr,                              // const void*            pNext;
    0,                                    // VkBufferCreateFlags    flags;
    size,                                 // VkDeviceSize           size;
    usages,                               // VkBufferUsageFlags     usage;
    VK_SHARING_MODE_EXCLUSIVE,            // VkSharingMode          sharingMode;
    0,                                    // uint32_t               queueFamilyIndexCount;
    nullptr                               // const uint32_t*        pQueueFamilyIndices;
  };

  auto res = devi.vkCreateBuffer(devi, &buffer_info, nullptr, &h_);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create buffer : " + vulkan::to_string(res)};
  }
}

buffer::~buffer() {
  if (memory_) {
    memory_->coalesce(block_);
    devi.vkDestroyBuffer(devi, h_, nullptr);
  }
}

void buffer::bind(memory& memory, memory::block_iterator block, uint64_t aligned) {
  assert(devi.vkBindBufferMemory(devi, h_, memory, aligned) == VK_SUCCESS
         && "Failed to bind buffer to memory");
  memory_ = &memory, block_ = block, aligned_ = aligned;
}

auto buffer::requirements(uint64_t& size, uint64_t& alignment) -> uint32_t {
  VkMemoryRequirements 
    mem_reqs{};
  devi.vkGetBufferMemoryRequirements(devi, h_, &mem_reqs);

  size = mem_reqs.size, alignment = mem_reqs.alignment;
  return mem_reqs.memoryTypeBits;
}

} // namespace ig
