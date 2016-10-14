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

#include "imagine/envi/impl_hw/device.h"

namespace ig {

device::device(const physical& physical, const std::vector<queue::types_t>& queues)
  : phys{physical}
  , inst{phys.inst} {

  prepare_queues(queues);
  std::vector<VkDeviceQueueCreateInfo> queues_info{};
  std::vector< std::vector<float> >    queue_prios{};
  for (auto& desc : queues_.descs) {
    queue_prios.emplace_back(desc.second.second, 1.f);
    VkDeviceQueueCreateInfo queue_info = {
      VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType             sType;
      nullptr,                                          // const void*                 pNext;
      0,                                                // VkDeviceQueueCreateFlags    flags;
      desc.first,                                       // uint32_t                    queueFamilyIndex;
      static_cast<uint32_t>(queue_prios.back().size()), // uint32_t                    queueCount;
      queue_prios.back().data()                         // const float*                pQueuePriorities;
    };
    queues_info.emplace_back(queue_info);
  }

  configure();
  std::vector<const char*> extensions(extensions_.size());
  std::transform(extensions_.begin(), extensions_.end(), extensions.begin(), [](auto& extension)
  { return extension.extensionName; });

  VkDeviceCreateInfo device_info = {
    VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,      // VkStructureType                    sType;
    nullptr,                                   // const void*                        pNext;
    0,                                         // VkDeviceCreateFlags                flags;
    static_cast<uint32_t>(queues_info.size()), // uint32_t                           queueCreateInfoCount;
    queues_info.data(),                        // const VkDeviceQueueCreateInfo*     pQueueCreateInfos;
    0,                                         // uint32_t                           enabledLayerCount;
    nullptr,                                   // const char* const*                 ppEnabledLayerNames;
    static_cast<uint32_t>(extensions.size()),  // uint32_t                           enabledExtensionCount;
    extensions.data(),                         // const char* const*                 ppEnabledExtensionNames;
    nullptr                                    // const VkPhysicalDeviceFeatures*    pEnabledFeatures;
  };

  auto res = inst.vkCreateDevice(phys, &device_info, nullptr, &h_);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create device : " + vulkan::to_string(res)};
  }

  vulkan::acquire(*this);
}

device::~device() {
  vkDestroyDevice(h_, nullptr);
}

bool device::wait() const {
  return vkDeviceWaitIdle(h_) == VK_SUCCESS;
}

bool device::supported(const std::string& name) const {
  return std::find_if(extensions_.begin(), extensions_.end(), [&name](auto& extension) {
    return std::string{extension.extensionName} == name;
  }) != extensions_.end();
}

void device::configure() {
  // Extensions
  uint32_t extension_count = 0;
  assert(inst.vkEnumerateDeviceExtensionProperties(phys, nullptr, &extension_count, nullptr) == VK_SUCCESS
         && "Failed to enumerate device extension properties");

  if (extension_count > 0) {
    extensions_.resize(extension_count);
    inst.vkEnumerateDeviceExtensionProperties(phys, nullptr, &extension_count, extensions_.data());
  }
}

void device::prepare_queues(const std::vector<queue::types_t>& queues) {
  auto& queues_families = phys.queues_families_;
  for (auto& request : queues) {
    int32_t fit = -1, candidate = -1;
    for (uint32_t i = 0; i < queues_families.size(); ++i) {
      auto& family = queues_families[i];
      if (queues_.descs.count(i) == family.queueCount) 
        continue;
      if (request == family.queueFlags) {
        fit = queues_.descs.count(fit) < queues_.descs.count(i) 
          ? fit 
          : i;
      } else if ((family.queueFlags & request) == request) {
        candidate = queues_.descs.count(candidate) < queues_.descs.count(i) 
          ? candidate 
          : i;
      }
    }
    uint32_t family = fit != -1 
      ? fit :
      candidate != -1 
        ? candidate 
        : 0;

    if (!queues_.descs.count(family)) {
      queues_.descs[family] = std::make_pair(queues_families[family], 1);
    } else {
      queues_.descs[family].second++;
    } queues_.indices.emplace_back(family, queues_.descs[family].second - 1);
  }
}

} // namespace ig
