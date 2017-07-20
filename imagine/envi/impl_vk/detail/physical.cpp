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
#include "imagine/envi/impl_vk/detail/physical.h"

namespace ig {
namespace vk {

struct physical::impl {
  // Describes fine-grained features that can be supported by the hardware
  // Interesting properties to forward:
  //   Shader support (geometryShader, tessellationShader)
  //   Images support (imageCubeArray)
  //   Drawing support (multiDrawIndirect, multiViewport)
  //   Pipeline support (depthClamp, depthBiasClamp, depthBounds, wideLines, largePoints)
  device_features features;

  // Describes global hardware properties and limits
  // Informations about API and driver versions
  // Device name, id and type, as well as vendor id
  // Hardware limits (shader stages, memory alignments, number of descriptors, etc.)
  VkPhysicalDeviceProperties properties; 
  // Describes memory heaps and associated types that can be allocated
  VkPhysicalDeviceMemoryProperties memory;

  // Describes queue families provided by the hardware
  // supported vkCmd* operations depends on the queue capacities (see spec.)
  std::vector<VkQueueFamilyProperties> queues_families; };

physical::physical(const instance& instance, VkPhysicalDevice_T* physical)
  : managed<VkPhysicalDevice_T*>{physical}
  , inst{instance}
  , impl_{std::make_unique<impl>()} 
{ postprocess(); }

physical::~physical() {}

int32_t physical::select_heap(uint32_t type, memory_properties properties) const {
  for (uint32_t i = 0; i < impl_->memory.memoryTypeCount; i++) {
    if ((type & 1) == 1) {
      if ((impl_->memory.memoryTypes[i].propertyFlags & properties) == properties)
        return i;
    } type >>= 1;
  }
  return -1;
}

int32_t physical::select_queue(capabilities caps) const {
  int32_t fit = -1, candidate = -1;
  for (uint32_t i = 0; i < impl_->queues_families.size() && fit < 0; ++i) {
    auto& family = impl_->queues_families[i];
    caps == family.queueFlags
      ? fit = i
      : (family.queueFlags & caps) == caps
        ? candidate = i
        : fit = fit;
  }
  return fit != -1 
    ? fit 
    : candidate != -1 
      ? candidate 
      : 0;
}

uint32_t physical::get_api_version() const
{ return impl_->properties.apiVersion; }
uint32_t physical::get_driver_version() const
{ return impl_->properties.driverVersion; }
uint32_t physical::get_id() const
{ return impl_->properties.deviceID; }
std::string physical::get_name() const
{ return impl_->properties.deviceName; }

hardware physical::get_type() const 
{ return static_cast<hardware>(impl_->properties.deviceType); }

size_t physical::get_ubo_alignment() const
{ return impl_->properties.limits.minUniformBufferOffsetAlignment; }
size_t physical::get_ssbo_alignment() const
{ return impl_->properties.limits.minStorageBufferOffsetAlignment; }

auto physical::get_features() const -> device_features
{ return impl_->features; }

void physical::postprocess() {
  inst->vkGetPhysicalDeviceFeatures(handle, &impl_->features);
  inst->vkGetPhysicalDeviceProperties(handle, &impl_->properties);
  inst->vkGetPhysicalDeviceMemoryProperties(handle, &impl_->memory);

  // Queue families
  uint32_t family_count = 0;
  inst->vkGetPhysicalDeviceQueueFamilyProperties(handle, &family_count, nullptr);
    impl_->queues_families.resize(family_count);
  inst->vkGetPhysicalDeviceQueueFamilyProperties(handle, &family_count, impl_->queues_families.data());
}

} // namespace vk
} // namespace ig
