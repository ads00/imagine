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

#include "imagine/envi/impl_hw/memory.h"
#include "imagine/envi/impl_hw/device.h"

namespace ig {

// memory
memory::memory(memory_allocator& allocator, uint64_t size)
  : allocator_{allocator}
  , size_{size}
  , mapped_{nullptr}
  , blocks_{block{0, size}}
  , frees_{{size_, blocks_.begin()}} {}

memory::~memory() {
  allocator_.devi.vkFreeMemory(allocator_.devi, h_, nullptr);
}

bool memory::map(resource& resource, uint64_t offset, uint64_t size) {
  assert(resource.memory_ == this && "Resource type is incoherent within device memory");
  assert(!mapped_ && "Device memory already mapped");
  return allocator_.devi.vkMapMemory(
    allocator_.devi, h_, resource.aligned_ + offset, size, 0, &mapped_) == VK_SUCCESS;
}

bool memory::remap(resource& resource, uint64_t offset, uint64_t size) {
  unmap();
  return map(resource, offset, size);
}

void memory::unmap() {
  if (mapped_) {
    allocator_.devi.vkUnmapMemory(allocator_.devi, h_);
    mapped_ = nullptr;
  }
}

void memory::coalesce(block_iterator block) {
  auto ub = frees_.upper_bound(block->offset);
  auto merge_prev = [&ub, this](auto& prev) {
    if (ub != frees_.end() && prev->offset + prev->size == ub->second->offset) {
      prev->size += ub->second->size; blocks_.erase(ub->second); 
      frees_.erase(ub);
    } frees_.emplace(prev->size, prev);
  };
  
  if (ub != frees_.begin()) {
    auto lb = std::prev(ub); auto prev = lb->second;
    if (prev->offset + prev->size == block->offset) {
      prev->size += block->size;
      blocks_.erase(block); frees_.erase(lb); merge_prev(prev);
    } else {
      block->distributed = false;
      frees_.emplace(block->size, block);
    }
  } else {
    block->distributed = false; merge_prev(block);
  }
}

// memory_allocator
memory_allocator::memory_allocator(const device& device)
  : devi{device}
  , phys{devi.phys} {

  phys.inst.vkGetPhysicalDeviceMemoryProperties(phys, &properties_);
}

memory_allocator::~memory_allocator() = default;

void memory_allocator::sweep() {
  for (auto& mems : pool_)
    mems.second.erase(std::remove_if(mems.second.begin(), mems.second.end(), [this](auto& mem) {
      return mem->blocks_.front().size == mem->size_;
    }), mems.second.end());
}

void memory_allocator::allocate_memory(uint64_t size, uint32_t index) {
  VkMemoryAllocateInfo memalloc_info = {
    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, // VkStructureType    sType;
    nullptr,                                // const void*        pNext;
    size,                                   // VkDeviceSize       allocationSize;
    index                                   // uint32_t           memoryTypeIndex;
  };

  auto mmr = std::make_shared<memory>(*this, memalloc_info.allocationSize);
  auto res = devi.vkAllocateMemory(devi, &memalloc_info, nullptr, *mmr);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to allocate device memory : " + vulkan::to_string(res)};
  }

  pool_[index].emplace_back(std::move(mmr));
}

uint32_t memory_allocator::get_index(uint32_t type, memory::types_t properties) const {
  for (uint32_t i = 0; i < properties_.memoryTypeCount; i++) {
    if ((type & i) == 1) {
      if ((properties_.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
    } type >>= 1;
  }
  return 0;
}

uint64_t memory_allocator::align(uint64_t offset, uint64_t alignment) const {
  return offset + alignment - 1 &~ (alignment - 1);
}

} // namespace ig
