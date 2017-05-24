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

#ifndef IG_ENVI_VK_BUFFER_H
#define IG_ENVI_VK_BUFFER_H

#include "imagine/envi/impl_vk/resource.h"

namespace ig {
namespace vk {

enum class index_type;
enum class buffer_usage : uint32_t; using buffer_usages = flags<buffer_usage>;

class ig_api buffer : public managed<VkBuffer_T*>, public resource {
public:
  using bview = VkBufferView_T*;

  explicit buffer(const device& device, uint64_t size, buffer_usages usages);
  virtual ~buffer();

  auto view(format format, uint64_t offset = 0, uint64_t range = ~0ull) 
    -> bview&;

private:
  bool bind(std::unique_ptr<block_type> block) override;
  auto require(uint64_t& size, uint64_t& alignment) -> uint32_t override;

private:
  std::unordered_map<format, bview> views_;
};

enum class index_type { 
  uint16 = 0, uint32 = 1 };

enum class buffer_usage : uint32_t {
  transfer_src  = 0x001, transfer_dst  = 0x002,
  uniform_texel = 0x004, storage_texel = 0x008,
  uniform       = 0x010, storage       = 0x020,
  index         = 0x040,
  vertex        = 0x080,
  indirect      = 0x100 };

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_BUFFER_H
