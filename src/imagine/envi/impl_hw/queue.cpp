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

#include "imagine/envi/impl_hw/queue.h"
#include "imagine/envi/impl_hw/device.h"

namespace ig {

queue::queue(const device& device, uint32_t i) 
  : device_{device}
  , cmdpool_{nullptr} {

  std::tie(family_, index_) = device_.queues_.indices[i];
  VkCommandPoolCreateInfo commandpool_info = {
    VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,      // VkStructureType             sType;
    nullptr,                                         // const void*                 pNext;
    VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, // VkCommandPoolCreateFlags    flags;
    family_                                          // uint32_t                    queueFamilyIndex;
  };

  auto res = device_.vkCreateCommandPool(device_, &commandpool_info, nullptr, &cmdpool_);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create commandpool : " + vulkan::to_string(res)};
  } 
  device_.vkGetDeviceQueue(device_, family_, index_, &h_);
}

queue::~queue() {
  wait();
  device_.vkDestroyCommandPool(device_, cmdpool_, nullptr);
}

bool queue::wait() const {
  return device_.vkQueueWaitIdle(h_) == VK_SUCCESS;
}

} // namespace ig
