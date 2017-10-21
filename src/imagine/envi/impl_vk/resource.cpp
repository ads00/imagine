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

#include "imagine/envi/impl_vk/resource.h"
#include "imagine/envi/impl_vk/detail/vulkan.h"

namespace ig {
namespace vk {

resource::resource(const device& device)
  : devi{device} {}

resource::~resource() {
  if (block_) { 
    auto& mem = block_->mem; mem.coalesce(std::move(block_)); }
}

bool resource::map(uint64_t offset, uint64_t size) {
  assert(block_ && "Resource not bound to device memory");
  assert(!block_->mem.mapped_ && "Device memory already mapped");
  return devi->vkMapMemory(
    devi, 
    block_->mem, 
    block_->offset + offset, 
    size, 
    0, 
    &block_->mem.mapped_) == VK_SUCCESS;
}

void resource::unm() {
  if (!block_->mem.mapped_) return;
  block_->mem.mapped_ = nullptr; 
  devi->vkUnmapMemory(
    devi, 
    block_->mem);
}

} // namespace vk
} // namespace ig
