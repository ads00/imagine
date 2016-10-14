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

#ifndef IG_ENVI_QUEUE_H
#define IG_ENVI_QUEUE_H

#include "imagine/envi/impl_hw/vulkan.h"
#include "imagine/envi/impl_hw/cmdbuffer.h"

#include <vector>

namespace ig {

class IG_API queue : public VKObject<VkQueue> {
public:
  enum type_t 
    : uint32_t; using types_t = flags<type_t>;

  explicit queue(const device& device, uint32_t i);
  virtual ~queue();

  bool wait() const;

  auto& family() const  { return family_; }
  auto& index() const   { return index_; }
  auto& cmdpool() const { return cmdpool_; }

  const device& device_;

private:
  uint32_t family_, index_;
  VkCommandPool cmdpool_;
};

enum queue::type_t : uint32_t {
  graphics = 0x001, compute = 0x002,
  transfer = 0x004, sparse  = 0x008
};

} // namespace ig

#endif // IG_ENVI_QUEUE_H
