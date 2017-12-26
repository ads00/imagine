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

#include "imagine/envi/impl_vk/surface.h"
#include "imagine/envi/impl_vk/detail/vulkan.h"

namespace ig {
namespace vk {

struct surface::impl {
  // Required surface capabilities for swapchain creation
  // Informations about images count, extents and composite-transform flags
  surface_capabilities capabilities;
  // Supported formats and present modes for this surface (adaptive capabilities)
  //   Immediate doesn't rely on queuing but may result in visible tearing
  //   Mailbox uses a single-entry queue that replace last existing entry with newers
  //   Fifo(_relaxed) are standard queues with swap intervals of 1 and -1 respectively
  std::vector<VkSurfaceFormatKHR> formats; std::vector<VkPresentModeKHR> present_modes; };

surface::surface(const physical& physical, const widget& widget)
  : phys{physical}
  , inst{phys.inst}
  , widg{widget}
  , impl_{std::make_unique<impl>()} {

  if (!inst.supported(VK_KHR_SURFACE_EXTENSION_NAME)) {
    throw std::runtime_error{"Failed to create surface : VK_KHR_surface extension unsupported"};
  }

  #if defined(IG_WIN)
  VkWin32SurfaceCreateInfoKHR surface_info {};
    surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surface_info.flags = 0;
    surface_info.hinstance = GetModuleHandle(nullptr);
    surface_info.hwnd      = reinterpret_cast<HWND>(widget.get_handle());

  auto res =
    inst->vkCreateWin32SurfaceKHR(
      inst,
      &surface_info,
      nullptr,
      &handle);
  #elif defined(IG_UNIX)
  VkXcbSurfaceCreateInfoKHR surface_info {};
    surface_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surface_info.flags = 0;
    // surface_info.connection = ;
    // surface_info.window = ;

  auto res =
    inst->vkCreateXcbSurfaceKHR(
      inst,
      &surface_info,
      nullptr,
      &handle);
  #endif

  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create surface : " + vulkan::to_string(res)};
  } else {
    get_queue();
    impl_get();
  }
}

surface::~surface() {
  inst->vkDestroySurfaceKHR(
    inst,
    handle,
    nullptr);
}

int32_t surface::get_queue() const {
  // Presentable queue
  uint32_t supported = false, i = 0;
  do
  { inst->vkGetPhysicalDeviceSurfaceSupportKHR(
      phys,
      i,
      handle,
      &supported); }
  while (!supported && ++i);
  return i;
}

auto surface::get_capabilities() const -> surface_capabilities
{ return impl_->capabilities; }

auto surface::support_format(format fmt) const -> format {
  auto fmt_it =
    std::find_if(
      impl_->formats.begin(),
      impl_->formats.end(),
      [this, &fmt](auto& format) { return format.format == enum_cast(fmt); });

  return fmt_it != impl_->formats.end()
    ? fmt
    : format::b8g8r8a8_unorm;
}

auto surface::support_present_mode(present_mode present) const -> present_mode {
  auto pres_it = std::find(impl_->present_modes.begin(), impl_->present_modes.end(), enum_cast(present));
  return pres_it != impl_->present_modes.end()
    ? present
    : present_mode::fifo;
}

void surface::impl_get() {
  // Capabilities
  inst->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    phys,
    handle,
    &impl_->capabilities);

  // Present modes
  uint32_t present_count = 0;
  inst->vkGetPhysicalDeviceSurfacePresentModesKHR(
    phys,
    handle,
    &present_count,
    nullptr);
    impl_->present_modes.resize(present_count);
  inst->vkGetPhysicalDeviceSurfacePresentModesKHR(
    phys,
    handle,
    &present_count,
    impl_->present_modes.data());
  // Formats
  uint32_t formats_count = 0;
  inst->vkGetPhysicalDeviceSurfaceFormatsKHR(
    phys,
    handle,
    &formats_count,
    nullptr);
    impl_->formats.resize(formats_count);
  inst->vkGetPhysicalDeviceSurfaceFormatsKHR(
    phys,
    handle,
    &formats_count,
    impl_->formats.data());
}

} // namespace vk
} // namespace ig
