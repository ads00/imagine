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

#include "imagine/envi/impl/window_impl.h"
#include "imagine/envi/impl_hw/surface.h"

namespace ig {

surface::surface(const physical& physical, types_t types, const std::string& caption, uint32_t w, uint32_t h)
  : window{types, caption, w, h}
  , phys{physical}
  , inst{phys.inst} {

  if (!inst.supported(VK_KHR_SURFACE_EXTENSION_NAME)) {
    throw std::runtime_error{"Failed to create surface : VK_KHR_surface extension unsupported"};
  }

  #if defined(IG_WIN)
  VkWin32SurfaceCreateInfoKHR surface_info = {
    VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, // VkStructureType                 sType;
    nullptr,                                         // const void*                     pNext;
    0,                                               // VkWin32SurfaceCreateFlagsKHR    flags;
    GetModuleHandle(nullptr),                        // HINSTANCE                       hinstance;
    reinterpret_cast<HWND>(window::handle())         // HWND                            hwnd;
  };
  auto res = inst.vkCreateWin32SurfaceKHR(inst, &surface_info, nullptr, &h_);
  #endif

  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create surface : " + vulkan::to_string(res)};
  }
}

surface::~surface() {
  inst.vkDestroySurfaceKHR(inst, h_, nullptr);
}

queue::types_t surface::queue() const {
  // Presentable queue
  auto& queues_families = phys.queues_families_;
  uint32_t supported = false, i = 0;
  do
  {
    assert(i != queues_families.size() && "No presentable queue available on this gpu");
    auto& family = queues_families[i];
    inst.vkGetPhysicalDeviceSurfaceSupportKHR(phys, i, h_, &supported);
  }
  while (!supported && ++i);
  return static_cast<queue::types_t>(queues_families[i].queueFlags);
}

void surface::acquire_capabilities() {
  // Capabilities
  assert(inst.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phys, h_, &capabilities_) == VK_SUCCESS
         && "Failed to retrieve surface capabilities");

  // Present modes
  uint32_t present_count = 0;
  assert(inst.vkGetPhysicalDeviceSurfacePresentModesKHR(phys, h_, &present_count, nullptr) == VK_SUCCESS
         && "Failed to retrieve surface present modes");

  if (present_count > 0) {
    present_modes_.resize(present_count);
    inst.vkGetPhysicalDeviceSurfacePresentModesKHR(phys, h_, &present_count, present_modes_.data());

    // Formats
    uint32_t formats_count = 0;
    assert(inst.vkGetPhysicalDeviceSurfaceFormatsKHR(phys, h_, &formats_count, nullptr) == VK_SUCCESS
           && "Failed to retrieve surface formats");

    if (formats_count > 0) {
      formats_.resize(formats_count);
      inst.vkGetPhysicalDeviceSurfaceFormatsKHR(phys, h_, &formats_count, formats_.data());
    }
  }
}

} // namespace ig
