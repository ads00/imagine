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

#ifndef IG_ENVI_VK_PHYSICAL_H
#define IG_ENVI_VK_PHYSICAL_H

#include "imagine/envi/impl_vk/detail/wrapper.h"
#include "imagine/envi/impl_vk/detail/instance.h"

namespace ig {
namespace vk {

class ig_api physical : public managed<VkPhysicalDevice_T*> {
public:
  explicit physical(const instance& instance, VkPhysicalDevice_T* physical);
  virtual ~physical();

  int32_t select_heap(uint32_t type, memory_properties properties) const;
  int32_t select_queue(queue_capabilities capabilities) const;

  // properties
  uint32_t get_api_version() const;
  uint32_t get_driver_version() const;
  uint32_t get_id() const;
  std::string get_name() const;

  hardware get_type() const;
  // limits
  size_t get_ubo_alignment() const;
  size_t get_ssbo_alignment() const;
  // vulkan
  auto get_features() const -> device_features;

  const instance& inst;

protected:
  virtual void post_acquire() override;

private:
  struct impl; 
  std::unique_ptr<impl> impl_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_PHYSICAL_H
