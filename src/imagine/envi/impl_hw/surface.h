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

#ifndef IG_ENVI_SURFACE_H
#define IG_ENVI_SURFACE_H

#include "imagine/envi/impl_hw/physical.h"
#include "imagine/envi/impl_hw/queue.h"
#include "imagine/envi/window.h"

#include <vector>

namespace ig {

class IG_API surface : public VKObject<VkSurfaceKHR>, public window {
public:
  explicit surface(const physical& physical, types_t types, const std::string& caption, uint32_t w, uint32_t h);
  virtual ~surface();

  queue::types_t queue() const;

  auto& capabilities() const { return capabilities_; }

  const physical& phys;
  const instance& inst;

private:
  void acquire_capabilities();

private:
  VkSurfaceCapabilitiesKHR capabilities_;
  std::vector<VkPresentModeKHR> present_modes_; std::vector<VkSurfaceFormatKHR> formats_;
};

} // namespace ig

#endif // IG_ENVI_SURFACE_H
