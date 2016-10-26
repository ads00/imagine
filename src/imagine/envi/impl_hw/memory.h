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

#ifndef IG_ENVI_MEMORY_H
#define IG_ENVI_MEMORY_H

#include "imagine/envi/impl_hw/vulkan.h"

#include <map>
#include <vector>
#include <list>
#include <unordered_map>

namespace ig {

class physical;
class IG_API memory : public VKObject<VkDeviceMemory> {
public:
  enum type_t 
    : uint32_t; using types_t = flags<type_t>;

  class resource;
  friend class memory_allocator;

  explicit memory(memory_allocator& allocator, uint64_t size);
  virtual ~memory();

  bool map(resource& resource, uint64_t offset = 0, uint64_t size = ~0ull);
  bool remap(resource& resource, uint64_t offset = 0, uint64_t size = ~0ull);
  void unmap();
  void* ptr() { return mapped_; }

  struct block { 
    explicit block(uint64_t offset, uint64_t size) 
      : offset{offset}
      , size{size}
      , distributed{false} {}
    uint64_t offset, size; bool distributed; 
  }; using block_iterator = std::list<block>::iterator;

  void coalesce(block_iterator block);

  class IG_API resource {
  public:
    friend memory;
    resource() : memory_{nullptr} {}

  private:
    virtual void bind(memory& memory, block_iterator block, uint64_t aligned) = 0;
    virtual auto requirements(uint64_t& size, uint64_t& alignment) -> uint32_t = 0;

  protected:
    memory* memory_; block_iterator block_;
    uint64_t aligned_;
  };

private:
  memory_allocator& allocator_; uint64_t size_; void* mapped_;
  std::list<block> blocks_;
  std::unordered_map<uint64_t, block_iterator> frees_;
};

class IG_API memory_allocator {
public:
  explicit memory_allocator(const device& device);
  ~memory_allocator();

  template <typename T>
  auto& bind(T& resource, memory::types_t properties) {
    uint64_t size, alignment; auto type = resource.requirements(size, alignment);
    return bind_memory(resource, size, alignment, get_index(type, properties));
  }

  void sweep();

  const device&   devi;
  const physical& phys;

private:
  /*
  static const uint64_t buffer_size = 33554432;
  static const uint64_t buffer_size = 16777216 */
  static const uint64_t buffer_size = 8388608;

  template <typename T>
  auto& bind_memory(T& resource, uint64_t size, uint64_t alignment, uint32_t index) {
    auto compose = [&resource](auto& mem, auto& block, uint64_t aligned, uint64_t aligned_end, uint64_t block_end) -> memory& {
      resource.bind(*mem, block, aligned);
      if (block_end > aligned_end) {
        auto remain = mem->blocks_.emplace(std::next(block), aligned_end, block_end - aligned_end);
                      mem->frees_ .emplace(remain->size, remain);
        block->size = aligned_end - block->offset;
      } 
      block->distributed = true;
      return *mem;
    };

    for (auto& mem : pool_[index]) {
      auto lb = mem->frees_.lower_bound(size);
      for (; lb != mem->frees_.end(); ++lb) {
        auto block = lb->second;
        auto aligned = align(block->offset, alignment),
          aligned_end = aligned + size, block_end = block->offset + block->size;
        if (block_end >= aligned_end) {
          mem->frees_.erase(lb);
          return compose(mem, block, aligned, aligned_end, block_end);
        }
      }
    }

    size > buffer_size 
      ? allocate_memory(size, index) 
      : allocate_memory(buffer_size, index);
    auto& memory = pool_[index].back(); auto first = memory->blocks_.begin();

    memory->frees_.clear();
    return compose(memory, first, 0, size, buffer_size);
  }

  void allocate_memory(uint64_t size, uint32_t index);

  uint32_t get_index(uint32_t type, memory::types_t properties) const;
  uint64_t align(uint64_t offset, uint64_t alignment) const;

private:
  VkPhysicalDeviceMemoryProperties properties_;
  std::map< uint32_t, std::vector< std::shared_ptr<memory> > > pool_;
};

enum memory::type_t : uint32_t {
  device_local = 0x001,
  host_visible = 0x002, host_coherent    = 0x004,
  host_cached  = 0x008, lazily_allocated = 0x010 };

} // namespace ig

#endif // IG_ENVI_MEMORY_H
