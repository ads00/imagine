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

#ifndef IG_ENVI_VK_WRAPPER_H
#define IG_ENVI_VK_WRAPPER_H

#include "imagine/ig.h"
#include "imagine/core/flags.h"

namespace ig {
namespace vk {

class vulkan;
class device;
class instance;

struct VkInstance_T;
struct VkPhysicalDevice_T;
struct VkDevice_T;
struct VkQueue_T;
struct VkSemaphore_T;
struct VkCommandBuffer_T;
struct VkFence_T;
struct VkDeviceMemory_T;
struct VkBuffer_T;
struct VkImage_T;
struct VkEvent_T;
struct VkQueryPool_T;
struct VkBufferView_T;
struct VkImageView_T;
struct VkShaderModule_T;
struct VkPipelineCache_T;
struct VkPipelineLayout_T;
struct VkRenderPass_T;
struct VkPipeline_T;
struct VkDescriptorSetLayout_T;
struct VkSampler_T;
struct VkDescriptorPool_T;
struct VkDescriptorSet_T;
struct VkFramebuffer_T;
struct VkCommandPool_T;

struct VkSurfaceKHR_T;
struct VkSwapchainKHR_T;
struct VkDisplayKHR_T;
struct VkDisplayModeKHR_T;
struct VkDescriptorUpdateTemplateKHR_T;

template <typename T>
class managed {
public:
  constexpr managed() : handle{nullptr} {}
  constexpr managed(T handle) 
    : handle{std::forward<T>(handle)} {}
  virtual ~managed() { handle = nullptr; }

  operator T*() { return &handle; }
  constexpr operator const T*() const { return &handle; }
  constexpr operator const T&() const { return handle; }

  managed(const managed&) = delete;
  managed& operator=(const managed&) = delete;

  managed(managed&& o) : handle{o.handle} { o.handle = nullptr; }
  managed& operator=(managed&& o) {
    handle = o.handle; o.handle = nullptr;
    return *this;
  }

protected:
  virtual void pre_acquire() {}
  virtual void post_acquire() {}
  T handle;
};

using device_features = struct VkPhysicalDeviceFeatures;
using surface_capabilities = struct VkSurfaceCapabilitiesKHR;

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_WRAPPER_H
