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

#include "imagine/envi/_vk/detail/vulkan.h"
#include "imagine/envi/_vk/detail/device.h"
#include "imagine/envi/_vk/memory.h"
#include "imagine/envi/_vk/surface.h"

namespace ig {
namespace vk {

struct device::impl {
  // Vulkan device extensions
  // Registered and published KHR(X) and EXT extensions supported
  // VK_KHR_swapchain rev. 68
  // VK_KHR_display_swapchain rev. 9 (draft)
  // VK_KHR_sampler_mirror_clamp_to_edge rev. 1 (final)
  // VK_KHR_shader_draw_parameters rev. 1 (complete)
  // VK_KHR_maintenance1 rev. 1 (draft)
  // VK_KHR_push_descriptor rev. 1 (draft)
  // VK_KHR_descriptor_update_template rev. 1 (in work)
  // VK_EXT_debug_marker rev. 4
  // VK_KHX_multiview rev. 1 (draft)
  // VK_KHX_device_group rev. 1 (draft)
  // VK_EXT_shader_subgroup_ballot rev. 1 (draft)
  // VK_EXT_shader_subgroup_vote rev. 1 (draft)
  // VK_EXT_display_control rev. 1
  // VK_EXT_discard_rectangles rev. 1 (complete)
  std::vector<VkExtensionProperties> extensions; };

device::device(const physical& physical, const std::vector<queue_capabilities>& rq)
  : phys{physical}
  , inst{phys.inst}
  , impl_{std::make_unique<impl>()}
  , dpfn_{std::make_unique<dPfn>()} {

  for (auto& request : rq)
    (request & queue_capability::present) != 0
      ? selects_.prepare(surface{phys, window{}}.get_queue())
      : selects_.prepare(phys.select_queue(request));

  std::vector<VkDeviceQueueCreateInfo> 
    queues_info{};
  std::vector< std::vector<float> >    
    queue_prios{};
  for (auto& definition : selects_.definitions) {
    queue_prios.emplace_back(definition.second, 1.f);
    VkDeviceQueueCreateInfo queue_info {};
      queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_info.flags = 0;
      queue_info.queueFamilyIndex = definition.first;
      queue_info.queueCount = static_cast<uint32_t>(queue_prios.back().size());
      queue_info.pQueuePriorities = queue_prios.back().data();

    queues_info.emplace_back(std::move(queue_info));
  }

  pre_acquire();
  std::vector<const char*> extensions(impl_->extensions.size());
  std::transform(impl_->extensions.begin(), impl_->extensions.end(), extensions.begin(), [](auto& extension)
  { return extension.extensionName; });

  VkDeviceCreateInfo device_info {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.flags = 0;
    device_info.pEnabledFeatures = &phys.get_features();
    device_info.queueCreateInfoCount = static_cast<uint32_t>(queues_info.size());
      device_info.pQueueCreateInfos  = queues_info.data();
    device_info.enabledExtensionCount     = static_cast<uint32_t>(extensions.size());
      device_info.ppEnabledExtensionNames = extensions.data();

  auto res = inst->vkCreateDevice(phys, &device_info, nullptr, &handle);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create device : " + vulkan::to_string(res)};
  } else {
    vulkan::acquire(*this);
    allocator_ = std::make_unique<memory_allocator>(*this);
  }
}

device::~device() {
  allocator_.reset(); 
  dpfn_->vkDestroyDevice(handle, nullptr);
}

bool device::wait() const {
  return dpfn_->vkDeviceWaitIdle(handle) == VK_SUCCESS;
}

bool device::supported(const std::string& name) const {
  return std::find_if(impl_->extensions.begin(), impl_->extensions.end(), [&name](auto& extension) {
    return std::string{extension.extensionName} == name;
  }) != impl_->extensions.end();
}

void device::pre_acquire() {
  // Extensions
  uint32_t extension_count = 0;
  assert(inst->vkEnumerateDeviceExtensionProperties(phys, nullptr, &extension_count, nullptr) == VK_SUCCESS
         && "Failed to enumerate device extension properties");

  if (extension_count > 0) {
    impl_->extensions.resize(extension_count);
    inst->vkEnumerateDeviceExtensionProperties(phys, nullptr, &extension_count, impl_->extensions.data()); }
}

void device::select::prepare(uint32_t family) {
  auto queue_it = std::find_if(definitions.begin(), definitions.end(), [&family](auto& definition) {
    return definition.first == family; 
  });

  queue_it = queue_it != definitions.end()
    ? queue_it
    : definitions.emplace(definitions.end(), family, 0);
  queue_it->second++;
  indices.emplace_back(family, queue_it->second - 1);
}

} // namespace vk
} // namespace ig
