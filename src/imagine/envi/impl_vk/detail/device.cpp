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

#include "imagine/envi/impl_vk/detail/vulkan.h"
#include "imagine/envi/impl_vk/detail/device.h"
#include "imagine/envi/impl_vk/surface.h"

namespace ig {
namespace vk {

struct device::impl {
  // Vulkan device extensions
  // Registered and published KHR and EXT extensions supported
  // VK_KHR_swapchain rev. 70 [James Jones, Ian Elliott]
  // VK_KHR_display_swapchain rev. 9 [James Jones]
  // VK_KHR_sampler_mirror_clamp_to_edge rev. 1 [Tobias Hector]
  // VK_KHR_multiview rev. 1 [Jeff Bolz]
  // VK_KHR_device_group rev. 3 [Jeff Bolz]
  // VK_KHR_shader_draw_parameters rev. 1 [Daniel Koch]
  // VK_KHR_maintenance1 rev. 2 [Piers Daniell]
  // VK_KHR_external_memory rev. 1 [James Jones]
  // VK_KHR_external_memory_fd rev. 1 [James Jones]
  // VK_KHR_external_semaphore rev. 1 [James Jones]
  // VK_KHR_external_semaphore_fd rev. 1 [James Jones]
  // VK_KHR_push_descriptor rev. 2 [Jeff Bolz]
  // VK_KHR_16bit_storage rev. 1 [Jan-Harald Fredriksen]
  // VK_KHR_incremental_present rev. 1 [Ian Elliott]
  // VK_KHR_descriptor_update_template rev. 1 [Markus Tavenrath]
  // VK_KHR_shared_presentable_image rev. 1 [Alon Or-back]
  // VK_KHR_external_fence rev. 1 [Jesse Hall]
  // VK_KHR_external_fence_fd rev. 1 [Jesse Hall]
  // VK_KHR_maintenance2 rev. 1 [Michael Worcester]
  // VK_KHR_variable_pointers rev. 1 [Jesse Hall]
  // VK_KHR_dedicated_allocation rev. 3 [James Jones]
  // VK_KHR_storage_buffer_storage_class rev. 1 [Alexander Galazin]
  // VK_KHR_relaxed_block_layout rev. 1 [John Kessenich]
  // VK_KHR_get_memory_requirements2 rev. 1 [Jason Ekstrand]
  // VK_KHR_image_format_list rev. 1 [Jason Ekstrand]
  // VK_KHR_sampler_ycbcr_conversion rev. 1 [Andrew Garrard]
  // VK_KHR_bind_memory2 rev. 1 [Tobias Hector]
  // VK_KHR_maintenance3 rev. 1 [Jeff Bolz]
  // VK_EXT_depth_range_unrestricted rev. 1 [Piers Daniell]
  // VK_EXT_debug_marker rev. 4 [Baldur Karlsson]
  // VK_EXT_shader_subgroup_ballot rev. 1 [Daniel Koch]
  // VK_EXT_shader_subgroup_vote rev. 1 [Daniel Koch]
  // VK_EXT_display_control rev. 1 [James Jones]
  // VK_EXT_discard_rectangles rev. 1 [Piers Daniell]
  // VK_EXT_conservative_rasterization rev. 1 [Piers Daniell]
  // VK_EXT_hdr_metadata rev. 1 [Courtney Goeltzenleuchter]
  // VK_EXT_external_memory_dma_buf rev. 1 [Chad Versace]
  // VK_EXT_queue_family_foreign rev. 1 [Chad Versace]
  // VK_EXT_sampler_filter_minmax rev. 1 [Jeff Bolz]
  // VK_EXT_sample_locations rev. 1 [Daniel Rakos]
  // VK_EXT_blend_operation_advanced rev. 2 [Jeff Bolz]
  // VK_EXT_post_depth_coverage rev. 1 [Jeff Bolz]
  // VK_EXT_validation_cache rev. 1 [Cort Stratton]
  // VK_EXT_shader_viewport_index_layer rev. 1 [Daniel Koch]
  // VK_EXT_global_priority rev. 2 [Andres Rodriguez]
  // VK_EXT_external_memory_host rev. 1 [Daniel Rakos]
  // VK_EXT_vertex_attribute_divisor rev. 1 [Vikram Kushwaha]
  std::vector<VkExtensionProperties> extensions; };

device::device(const physical& physical, const std::vector<capabilities>& rq)
  : phys{physical}
  , inst{phys.inst}
  , impl_{std::make_unique<impl>()}
  , dpfn_{std::make_unique<dPfn>()} {

  auto features = phys.get_features();
  for (auto& request : rq)
    (request & capability::present) != 0
      ? selects_.prepare(surface{phys, widget{}}.get_queue())
      : selects_.prepare(phys.queue(request));

  std::vector<VkDeviceQueueCreateInfo>
    queues_info{};
  std::vector
  < std::vector<float>
  > queue_prios{};
  for (auto& [family, count] : selects_.definitions) {
    auto& prio = queue_prios.emplace_back(count, 1.f);
    VkDeviceQueueCreateInfo queue_info {};
      queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_info.flags = 0;
      queue_info.queueFamilyIndex = family;
      queue_info.queueCount = static_cast<uint32_t>(prio.size());
      queue_info.pQueuePriorities = prio.data();

    queues_info.emplace_back(std::move(queue_info));
  }

  impl_get();
  std::vector<const char*> extensions(impl_->extensions.size());
  std::transform(
    impl_->extensions.begin(),
    impl_->extensions.end(),
    extensions.begin(),
    [](auto& extension) { return extension.extensionName; });

  VkDeviceCreateInfo device_info {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.flags = 0;
    device_info.pEnabledFeatures = &features;
    device_info.queueCreateInfoCount = static_cast<uint32_t>(queues_info.size());
      device_info.pQueueCreateInfos  = queues_info.data();
    device_info.enabledExtensionCount     = static_cast<uint32_t>(extensions.size());
      device_info.ppEnabledExtensionNames = extensions.data();

  auto res =
    inst->vkCreateDevice(
      phys,
      &device_info,
      nullptr,
      &handle);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create device : " + vulkan::to_string(res)};
  } else {
    vulkan::acquire_fn(*this);
  }
}

device::~device() {
  dpfn_->vkDestroyDevice(
    handle,
    nullptr);
}

bool device::wait() const {
  return dpfn_->vkDeviceWaitIdle(handle) == VK_SUCCESS;
}

bool device::supported(const std::string& name) const {
  return std::find_if(
    impl_->extensions.begin(),
    impl_->extensions.end(),
    [&name](auto& ext) { return std::string{ext.extensionName} == name; }) != impl_->extensions.end();
}

void device::impl_get() {
  // Extensions
  uint32_t extension_count = 0;
  assert(
    inst->vkEnumerateDeviceExtensionProperties(
      phys,
      nullptr,
      &extension_count,
      nullptr) == VK_SUCCESS
    && "Failed to enumerate device extension properties");

  if (extension_count > 0) {
    impl_->extensions.resize(extension_count);
    inst->vkEnumerateDeviceExtensionProperties(
      phys,
      nullptr,
      &extension_count,
      impl_->extensions.data());
  }
}

void device::select::prepare(uint32_t family) {
  auto queue_it =
    std::find_if(
      definitions.begin(),
      definitions.end(),
      [&family](auto& definition) { return definition.first == family; });

  if (queue_it == definitions.end())
    queue_it =
      definitions.emplace(
        definitions.end(),
        family,
        1);
  else queue_it->second++;
  indices.emplace_back(family, queue_it->second - 1);
}

} // namespace vk
} // namespace ig
