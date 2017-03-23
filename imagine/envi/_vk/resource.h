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

#ifndef IG_ENVI_VK_RESOURCE_H
#define IG_ENVI_VK_RESOURCE_H

#include "imagine/envi/_vk/memory.h"

namespace ig {
namespace vk {

class ig_api resource {
public:
  friend memory_allocator;
  using block_type = memory::block;

  explicit resource(const device& device);
  virtual ~resource();

  bool map(uint64_t offset = 0, uint64_t size = ~0ull);
  void unm();
  // bool fill(const void* data, uint64_t offset = 0, uint64_t size = ~0ull);

  void* get_ptr() { return block_->mem.mapped_; }

  const device& devi;

private:
  virtual bool bind(std::unique_ptr<block_type> block) = 0;
  virtual auto require(uint64_t& size, uint64_t& alignment) -> uint32_t = 0;

protected:
  std::unique_ptr<block_type> block_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_RESOURCE_H
