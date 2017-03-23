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

#include "imagine/envi/_vk/detail/device.h"

#include <unordered_map>

namespace ig {
namespace vk {

class ig_api memory : public managed<VkDeviceMemory_T*> {
public:
  friend class resource;
  friend class memory_allocator;

  explicit memory(memory_allocator& allocator, uint32_t fl, uint32_t sl);
  virtual ~memory();

  struct block {
    explicit block(memory& memory, uint64_t size, uint64_t offset, bool free)
      : mem{memory}
      , size{size}
      , offset{offset}, free{free} {}
    ~block() {}

    memory& mem;
    uint64_t size, offset; bool free;
    using block_p = block*; block_p prev_block = nullptr, next_block = nullptr;
  };

protected:
  auto allocate(uint64_t size, uint64_t alignment) -> std::unique_ptr<block>;
  void coalesce(std::unique_ptr<block> block);

  auto mapping_insert(uint32_t size) {
    auto fl = fls(size), sl = size >> (fl - layer_size_) ^ (1 << layer_size_);
    return std::make_pair(fl - (layer_size_ + 1), sl); }
  auto mapping_search(uint32_t size) {
    return mapping_insert(size + (1 << (fls(size) - layer_size_)) - 1); }

  auto manage(uint64_t size, uint64_t offset, bool free) -> std::unique_ptr<block>&;

  static uint32_t ffs(uint32_t v);
  static uint32_t fls(uint32_t v);

  const device& devi;

private:
   void* mapped_;
   block sentinel_; uint32_t layer_size_;
   std::vector<uint32_t> layers_bitmap_; std::vector< std::vector< std::unique_ptr<block> > > blocks_;
};

class ig_api memory_allocator {
public:
  explicit memory_allocator(const device& device);
  ~memory_allocator();

  void bind(resource& resource, memory_properties properties);
  void allocate(uint32_t log2size, uint32_t index);

  memory_allocator(const memory_allocator&) = delete;
  memory_allocator& operator=(const memory_allocator&) = delete;

  const device&   devi;
  const physical& phys;

private:
  /*
  static const uint32_t log2_size = 25; */
  static const uint32_t log2_size = 24;
  static const uint32_t log2_layers = 4;

private:
  std::unordered_map< uint32_t, std::vector< std::unique_ptr<memory> > > pool_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_MEMORY_H
