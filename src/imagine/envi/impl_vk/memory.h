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

#ifndef IG_ENVI_VK_MEMORY_H
#define IG_ENVI_VK_MEMORY_H

#include "imagine/envi/impl_vk/detail/device.h"

#include <unordered_map>

namespace ig {
namespace vk {

class IG_API memory : public managed<VkDeviceMemory_T*> {
public:
  friend class resource;
  friend class memory_allocator;

  explicit memory(memory_allocator& allocator);
  virtual ~memory();

  struct block {
    explicit block(memory& memory, uint64_t size, uint64_t offset, bool free)
      : mem{memory}
      , size{size}
      , offset{offset}, free{free} {}
    ~block() {}

    block* prev_block = nullptr, *next_block = nullptr;
    memory& mem;
    uint64_t size, offset; 
    bool free;
  }; using block_ptr = std::unique_ptr<block>;

protected:
  /*
  static const uint32_t log2_size = 25; */
  static const uint32_t log2_size = 24;
  static const uint32_t log2_layers = 4;

protected:
  auto allocate(uint64_t size, uint64_t alignment) -> block_ptr;
  void coalesce(block_ptr block);

  auto mapping_insert(uint32_t size) {
    auto fl = fls(size), sl = size >> (fl - log2_layers) ^ (1 << log2_layers);
    return std::make_pair(fl - (log2_layers + 1), sl); }
  auto mapping_search(uint32_t size) {
    return mapping_insert(size + (1 << (fls(size) - log2_layers)) - 1); }

  auto manage(uint64_t size, uint64_t offset, bool free) -> block_ptr&;

  static uint32_t ffs(uint32_t v);
  static uint32_t fls(uint32_t v);

  const device& devi;

private:
  block sentinel_;
  void* mapped_;
  std::vector<uint32_t> layers_bitmap_; 
  std::vector
  < std::vector<block_ptr>
  > blocks_;
};

class IG_API memory_allocator {
public:
  explicit memory_allocator(const device& device);
  ~memory_allocator();

  void bind(resource& resource, memory_properties properties);
  void allocate(uint32_t index);

  memory_allocator(const memory_allocator&) = delete;
  memory_allocator& operator=(const memory_allocator&) = delete;

  const device&   devi;
  const physical& phys;

private:
  using memory_ptr = std::unique_ptr<memory>;
  std::unordered_map
  < 
    uint32_t, 
    std::vector<memory_ptr>
  > pools_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_MEMORY_H
