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

#include "imagine/envi/_vk/memory.h"
#include "imagine/envi/_vk/resource.h"
#include "imagine/envi/_vk/detail/vulkan.h"

namespace ig {
namespace vk {

constexpr uint32_t log2_min_blocksize = 5;
constexpr uint64_t      min_blocksize = 32;

// memory
memory::memory(memory_allocator& allocator, uint32_t fl, uint32_t sl)
  : devi{allocator.devi}
  , sentinel_{*this, 0, 0, false}
  , mapped_{nullptr}
  , layer_size_{sl}
  , layers_bitmap_(fl - log2_min_blocksize + 1, 0)
  , blocks_      ((fl - log2_min_blocksize) * (1 << layer_size_)) {

  auto& pool_block = manage((1 << fl) - 1, 0, true);
  sentinel_.prev_block = sentinel_.next_block = 
    pool_block->prev_block = pool_block->next_block = &sentinel_;
}

memory::~memory() {
  devi->vkFreeMemory(devi, handle, nullptr);
}

auto memory::allocate(uint64_t size, uint64_t alignment) -> std::unique_ptr<block> {
  auto map = mapping_search(static_cast<uint32_t>
    (ig::align(std::max(size, min_blocksize), alignment)));
  auto map_sl = layers_bitmap_[map.first + 1] & (~0 << map.second);
  if (!map_sl) {
    auto map_fl = layers_bitmap_.front() & (~0 << (map.first + 1));
    if (!map_fl)
      return nullptr;
    map.first = ffs(map_fl); map_sl = layers_bitmap_[map.first + 1];
  }

  map.second = ffs(map_sl);
  auto& seg_list = blocks_[map.first * (1 << layer_size_) + map.second];
  auto block = std::move(seg_list.back());

  seg_list.pop_back();
  if (seg_list.empty())
    layers_bitmap_[map.first + 1] &= ~(1 << map.second);
  if (!layers_bitmap_[map.first + 1])
    layers_bitmap_.front() &= ~(1 << map.first);

  auto link_before = [&block](auto& next) {
    next->next_block = block.get(); next->prev_block = block->prev_block;
    block->prev_block = block->prev_block->next_block = next.get(); };
  auto link_after  = [&block](auto& prev) {
    prev->prev_block = block.get(); prev->next_block = block->next_block;
    block->next_block = block->next_block->prev_block = prev.get(); };

  auto aligned_offset = ig::align(block->offset, alignment);
  auto remain = aligned_offset - block->offset;
  if (remain) {
    if (remain > min_blocksize) {
      link_before(manage(remain, block->offset, true));
      block->size -= remain; block->offset += remain;
    } else {
      block->prev_block->size += remain; block->size -= remain; block->offset = aligned_offset;
    }
  }

  if (block->size - size >= min_blocksize) {
    link_after(manage(block->size - size, block->offset + size, true));
    block->size = size;
  }
    
  block->free = false;
  return block;
}

void memory::coalesce(std::unique_ptr<block> block) {
  auto coalesce_neighbor = [this, &block](auto& neighbor, bool prior) {
    auto mapping = mapping_insert(static_cast<uint32_t>(neighbor->size));
    auto& seg_list = blocks_[mapping.first * (1 << layer_size_) + mapping.second];

    auto& block_it = std::find_if(seg_list.begin(), seg_list.end(), [&block, &neighbor](auto& b) {
      return b.get() == neighbor; });

    block->offset = std::min(block->offset, neighbor->offset);
    block->size  += neighbor->size;

    prior 
      ? block->prev_block = neighbor->prev_block, block->prev_block->next_block = block.get()
      : block->next_block = neighbor->next_block, block->next_block->prev_block = block.get();

    seg_list.erase(block_it);
    if (seg_list.empty())
      layers_bitmap_[mapping.first + 1] &= ~(1 << mapping.second);
    if (!layers_bitmap_[mapping.first + 1])
      layers_bitmap_.front() &= ~(1 << mapping.first);
  };
    
  block->free = true;
  if (block->prev_block->free) coalesce_neighbor(block->prev_block, true);
  if (block->next_block->free) coalesce_neighbor(block->next_block, false);

  auto mapping = mapping_insert(static_cast<uint32_t>(block->size));
  auto& seg_list = blocks_[mapping.first * (1 << layer_size_) + mapping.second];

  layers_bitmap_.front()            |= (1 << mapping.first);
  layers_bitmap_[mapping.first + 1] |= (1 << mapping.second);
  seg_list.emplace(
    seg_list.end(), std::move(block));
}

auto memory::manage(uint64_t size, uint64_t offset, bool free) -> std::unique_ptr<block>& {
  auto mapping = mapping_insert(static_cast<uint32_t>(size));
  auto& seg_list = blocks_[mapping.first * (1 << layer_size_) + mapping.second];

  layers_bitmap_.front() |= (1 << mapping.first);
  layers_bitmap_[mapping.first + 1] |= (1 << mapping.second);
  return *seg_list.emplace(
    seg_list.end(), std::make_unique<block>(*this, size, offset, free));
}

auto find_bitset(uint32_t v) {
  uint32_t 
    bits = 32;
  if (!(v & 0xffff0000)) v <<= 16, bits -= 16; if (!(v & 0xff000000)) v <<=  8, bits -=  8;
  if (!(v & 0xf0000000)) v <<=  4, bits -=  4; if (!(v & 0xc0000000)) v <<=  2, bits -=  2;
  if (!(v & 0x80000000)) v <<=  1, bits -=  1; return bits;
}

uint32_t memory::ffs(uint32_t v) { return find_bitset(v & (~v + 1)) - 1; }
uint32_t memory::fls(uint32_t v) { return find_bitset(v) - 1; }

// memory_allocator
memory_allocator::memory_allocator(const device& device)
  : devi{device}
  , phys{devi.phys} {}

memory_allocator::~memory_allocator() = default;

void memory_allocator::bind(resource& resource, memory_properties properties) {
  uint64_t size, alignment; 
  auto type = resource.require(size, alignment); 
  auto heap = phys.select_heap(type, properties);

  auto block_it = std::find_if(pool_[heap].begin(), pool_[heap].end(), [&resource, &size, &alignment](auto& mem) {
    auto block = mem->allocate(size, alignment);
    return block
      ? resource.bind(std::move(block))
      : false;
  });

  if (block_it == pool_[heap].end()) {
    allocate(log2_size, heap);
    auto& mem = pool_[heap].back();
    auto block = mem->allocate(size, alignment); resource.bind(std::move(block));
  }
}

void memory_allocator::allocate(uint32_t log2size, uint32_t index) {
  VkMemoryAllocateInfo memalloc_info {};
    memalloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memalloc_info.allocationSize  = 1ull << log2size;
    memalloc_info.memoryTypeIndex = index;

  auto mmr = std::make_unique<memory>(*this, log2size, log2_layers);
  auto res = devi->vkAllocateMemory(devi, &memalloc_info, nullptr, *mmr);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to allocate device memory : " + vulkan::to_string(res)};
  } else {
    pool_[index].emplace_back(std::move(mmr));
  }
}

} // namespace vk
} // namespace ig
