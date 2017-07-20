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

#ifndef IG_ENVI_VK_SURFACE_H
#define IG_ENVI_VK_SURFACE_H

#include "imagine/envi/window.h"
#include "imagine/envi/impl_vk/detail/physical.h"

namespace ig {
namespace vk {

enum class present_mode { immediate = 0, mailbox = 1, fifo = 2, fifo_relaxed = 3 };

class display;
class ig_api surface : public managed<VkSurfaceKHR_T*> {
public:
  friend class swapchain;

  explicit surface(const physical& physical, const window& window);
  virtual ~surface();

  int32_t get_queue() const;
  // vulkan
  auto get_capabilities() const -> surface_capabilities;

  const physical& phys;
  const instance& inst;
  const window&   wind;

protected:
  auto support_format(format fmt) const -> format;
  auto support_present_mode(present_mode present) const -> present_mode;

  virtual void postprocess() override;

private:
  struct impl; 
  std::unique_ptr<impl> impl_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_SURFACE_H
