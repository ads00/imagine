/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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

template <typename Handle>
class managed {
public:
  constexpr managed() = default;
  constexpr managed(Handle handle)
    : handle{handle} {}

  constexpr operator const Handle&() const { return handle; }
  constexpr operator const Handle*() const { return &handle; }
  operator Handle*()
  { return &handle; }

  managed(const managed&) = delete;
  managed& operator=(const managed&) = delete;

  managed(managed&& o) : handle{o.handle} { o.handle = nullptr; }
  managed& operator=(managed&& o) {
    handle = o.handle; o.handle = nullptr;
    return *this;
  }

protected:
  virtual ~managed() { handle = nullptr; }
  Handle handle = nullptr;
};

using device_features      = struct VkPhysicalDeviceFeatures;
using surface_capabilities = struct VkSurfaceCapabilitiesKHR;

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_WRAPPER_H
