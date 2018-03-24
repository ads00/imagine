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

#ifndef IG_ENVI_VK_VULKAN_H
#define IG_ENVI_VK_VULKAN_H

#include "imagine/math/basis.h"
#include "imagine/envi/library.h"
#include "imagine/envi/impl_arch/config_impl.h"

#if defined(IG_WIN)
 #define VK_DYN_LOADER "vulkan-1.dll"
 #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(IG_UNIX)
 #define VK_DYN_LOADER "libvulkan.so.1"
 #define VK_USE_PLATFORM_XCB_KHR
#endif

namespace ig {
namespace vk {

#define  VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

class instance;
class device;
class vulkan {
public:
  friend instance;
  friend device;

  vulkan() = delete;
  static void discover();
  static std::string to_string(VkResult res);

protected:
  static void acquire_fn(instance& i); static void acquire_fn(device& d);
  static library vulkan_;
};

// VK_core
extern IG_API PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;
extern IG_API PFN_vkCreateInstance                       vkCreateInstance;
extern IG_API PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern IG_API PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;

class IG_API iPfn {
public:
  // VK_instance_core
  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
  PFN_vkGetDeviceProcAddr   vkGetDeviceProcAddr   = nullptr;

  PFN_vkDestroyInstance                              vkDestroyInstance                              = nullptr;
  PFN_vkEnumeratePhysicalDevices                     vkEnumeratePhysicalDevices                     = nullptr;
  PFN_vkGetPhysicalDeviceFeatures                    vkGetPhysicalDeviceFeatures                    = nullptr;
  PFN_vkGetPhysicalDeviceFormatProperties            vkGetPhysicalDeviceFormatProperties            = nullptr;
  PFN_vkGetPhysicalDeviceImageFormatProperties       vkGetPhysicalDeviceImageFormatProperties       = nullptr;
  PFN_vkGetPhysicalDeviceProperties                  vkGetPhysicalDeviceProperties                  = nullptr;
  PFN_vkGetPhysicalDeviceQueueFamilyProperties       vkGetPhysicalDeviceQueueFamilyProperties       = nullptr;
  PFN_vkGetPhysicalDeviceMemoryProperties            vkGetPhysicalDeviceMemoryProperties            = nullptr;
  PFN_vkCreateDevice                                 vkCreateDevice                                 = nullptr;
  PFN_vkEnumerateDeviceExtensionProperties           vkEnumerateDeviceExtensionProperties           = nullptr;
  PFN_vkEnumerateDeviceLayerProperties               vkEnumerateDeviceLayerProperties               = nullptr;
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;

  // VK_KHR_surface
  PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
  // VK_KHR_display
  PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR = nullptr;
  PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR = nullptr;
  PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR = nullptr;
  PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR = nullptr;
  PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR = nullptr;
  PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR = nullptr;
  PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR = nullptr;
  // VK_KHR_get_physical_device_properties2
  PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR = nullptr;
  PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR = nullptr;
  // VK_KHR_device_group_creation
  PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR = nullptr;
  // VK_KHR_external_memory_capabilities
  PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR = nullptr;
  // VK_KHR_external_semaphore_capabilities
  PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = nullptr;
  // VK_KHR_external_fence_capabilities
  PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR = nullptr;
  // VK_KHR_get_surface_capabilities2
  PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR = nullptr;
  // VK_EXT_debug_report
  PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = nullptr;
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;
  PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT = nullptr;
  // VK_EXT_direct_mode_display
  PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT = nullptr;
  // VK_EXT_display_surface_counter
  PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT = nullptr;
  // VK_EXT_debug_utils
  PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = nullptr;
  PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT = nullptr;
  PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT = nullptr;
  PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT = nullptr;
  PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT = nullptr;
  PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT = nullptr;
  PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT = nullptr;
  PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT = nullptr;
  PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = nullptr;
  PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;
  PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT = nullptr;

  #if defined(VK_USE_PLATFORM_XLIB_KHR)
  // VK_KHR_xlib_surface
  PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_XCB_KHR)
  // VK_KHR_xcb_surface
  PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_WAYLAND_KHR)
  // VK_KHR_wayland_surface
  PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_MIR_KHR)
  // VK_KHR_mir_surface
  PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_WIN32_KHR)
  // VK_KHR_win32_surface
  PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_ANDROID_KHR)
  // VK_KHR_android_surface
  PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_MACOS_MVK)
  // VK_MVK_macos_surface
  PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_IOS_MVK)
  // VK_MVK_ios_surface
  PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_VI_NN)
  // VK_NN_vi_surface
  PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN = nullptr;
  #endif
};

class IG_API dPfn {
public:
  // VK_device_core
  PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;

  PFN_vkDestroyDevice  vkDestroyDevice  = nullptr;
  PFN_vkGetDeviceQueue vkGetDeviceQueue = nullptr;
  PFN_vkQueueSubmit    vkQueueSubmit    = nullptr;
  PFN_vkQueueWaitIdle  vkQueueWaitIdle  = nullptr;
  PFN_vkDeviceWaitIdle vkDeviceWaitIdle = nullptr;

  PFN_vkAllocateMemory                   vkAllocateMemory                   = nullptr;
  PFN_vkFreeMemory                       vkFreeMemory                       = nullptr;
  PFN_vkMapMemory                        vkMapMemory                        = nullptr;
  PFN_vkUnmapMemory                      vkUnmapMemory                      = nullptr;
  PFN_vkFlushMappedMemoryRanges          vkFlushMappedMemoryRanges          = nullptr;
  PFN_vkInvalidateMappedMemoryRanges     vkInvalidateMappedMemoryRanges     = nullptr;
  PFN_vkGetDeviceMemoryCommitment        vkGetDeviceMemoryCommitment        = nullptr;
  PFN_vkBindBufferMemory                 vkBindBufferMemory                 = nullptr;
  PFN_vkBindImageMemory                  vkBindImageMemory                  = nullptr;
  PFN_vkGetBufferMemoryRequirements      vkGetBufferMemoryRequirements      = nullptr;
  PFN_vkGetImageMemoryRequirements       vkGetImageMemoryRequirements       = nullptr;
  PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements = nullptr;
  PFN_vkQueueBindSparse                  vkQueueBindSparse                  = nullptr;

  PFN_vkCreateFence      vkCreateFence      = nullptr;
  PFN_vkDestroyFence     vkDestroyFence     = nullptr;
  PFN_vkResetFences      vkResetFences      = nullptr;
  PFN_vkGetFenceStatus   vkGetFenceStatus   = nullptr;
  PFN_vkWaitForFences    vkWaitForFences    = nullptr;
  PFN_vkCreateSemaphore  vkCreateSemaphore  = nullptr;
  PFN_vkDestroySemaphore vkDestroySemaphore = nullptr;
  PFN_vkCreateEvent      vkCreateEvent      = nullptr;
  PFN_vkDestroyEvent     vkDestroyEvent     = nullptr;
  PFN_vkGetEventStatus   vkGetEventStatus   = nullptr;
  PFN_vkSetEvent         vkSetEvent         = nullptr;
  PFN_vkResetEvent       vkResetEvent       = nullptr;

  PFN_vkCreateQueryPool     vkCreateQueryPool     = nullptr;
  PFN_vkDestroyQueryPool    vkDestroyQueryPool    = nullptr;
  PFN_vkGetQueryPoolResults vkGetQueryPoolResults = nullptr;

  PFN_vkCreateBuffer              vkCreateBuffer              = nullptr;
  PFN_vkDestroyBuffer             vkDestroyBuffer             = nullptr;
  PFN_vkCreateBufferView          vkCreateBufferView          = nullptr;
  PFN_vkDestroyBufferView         vkDestroyBufferView         = nullptr;
  PFN_vkCreateImage               vkCreateImage               = nullptr;
  PFN_vkDestroyImage              vkDestroyImage              = nullptr;
  PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout = nullptr;
  PFN_vkCreateImageView           vkCreateImageView           = nullptr;
  PFN_vkDestroyImageView          vkDestroyImageView          = nullptr;
  PFN_vkCreateShaderModule        vkCreateShaderModule        = nullptr;
  PFN_vkDestroyShaderModule       vkDestroyShaderModule       = nullptr;

  PFN_vkCreatePipelineCache     vkCreatePipelineCache     = nullptr;
  PFN_vkDestroyPipelineCache    vkDestroyPipelineCache    = nullptr;
  PFN_vkGetPipelineCacheData    vkGetPipelineCacheData    = nullptr;
  PFN_vkMergePipelineCaches     vkMergePipelineCaches     = nullptr;
  PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = nullptr;
  PFN_vkCreateComputePipelines  vkCreateComputePipelines  = nullptr;
  PFN_vkDestroyPipeline         vkDestroyPipeline         = nullptr;
  PFN_vkCreatePipelineLayout    vkCreatePipelineLayout    = nullptr;
  PFN_vkDestroyPipelineLayout   vkDestroyPipelineLayout   = nullptr;

  PFN_vkCreateSampler              vkCreateSampler              = nullptr;
  PFN_vkDestroySampler             vkDestroySampler             = nullptr;
  PFN_vkCreateDescriptorSetLayout  vkCreateDescriptorSetLayout  = nullptr;
  PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout = nullptr;
  PFN_vkCreateDescriptorPool       vkCreateDescriptorPool       = nullptr;
  PFN_vkDestroyDescriptorPool      vkDestroyDescriptorPool      = nullptr;
  PFN_vkResetDescriptorPool        vkResetDescriptorPool        = nullptr;
  PFN_vkAllocateDescriptorSets     vkAllocateDescriptorSets     = nullptr;
  PFN_vkFreeDescriptorSets         vkFreeDescriptorSets         = nullptr;
  PFN_vkUpdateDescriptorSets       vkUpdateDescriptorSets       = nullptr;

  PFN_vkCreateFramebuffer        vkCreateFramebuffer        = nullptr;
  PFN_vkDestroyFramebuffer       vkDestroyFramebuffer       = nullptr;
  PFN_vkCreateRenderPass         vkCreateRenderPass         = nullptr;
  PFN_vkDestroyRenderPass        vkDestroyRenderPass        = nullptr;
  PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity = nullptr;

  PFN_vkCreateCommandPool      vkCreateCommandPool      = nullptr;
  PFN_vkDestroyCommandPool     vkDestroyCommandPool     = nullptr;
  PFN_vkResetCommandPool       vkResetCommandPool       = nullptr;
  PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = nullptr;
  PFN_vkFreeCommandBuffers     vkFreeCommandBuffers     = nullptr;
  PFN_vkBeginCommandBuffer     vkBeginCommandBuffer     = nullptr;
  PFN_vkEndCommandBuffer       vkEndCommandBuffer       = nullptr;
  PFN_vkResetCommandBuffer     vkResetCommandBuffer     = nullptr;

  // VK_cmd
  PFN_vkCmdBindPipeline           vkCmdBindPipeline           = nullptr;
  PFN_vkCmdSetViewport            vkCmdSetViewport            = nullptr;
  PFN_vkCmdSetScissor             vkCmdSetScissor             = nullptr;
  PFN_vkCmdSetLineWidth           vkCmdSetLineWidth           = nullptr;
  PFN_vkCmdSetDepthBias           vkCmdSetDepthBias           = nullptr;
  PFN_vkCmdSetBlendConstants      vkCmdSetBlendConstants      = nullptr;
  PFN_vkCmdSetDepthBounds         vkCmdSetDepthBounds         = nullptr;
  PFN_vkCmdSetStencilCompareMask  vkCmdSetStencilCompareMask  = nullptr;
  PFN_vkCmdSetStencilWriteMask    vkCmdSetStencilWriteMask    = nullptr;
  PFN_vkCmdSetStencilReference    vkCmdSetStencilReference    = nullptr;
  PFN_vkCmdBindDescriptorSets     vkCmdBindDescriptorSets     = nullptr;
  PFN_vkCmdBindIndexBuffer        vkCmdBindIndexBuffer        = nullptr;
  PFN_vkCmdBindVertexBuffers      vkCmdBindVertexBuffers      = nullptr;
  PFN_vkCmdDraw                   vkCmdDraw                   = nullptr;
  PFN_vkCmdDrawIndexed            vkCmdDrawIndexed            = nullptr;
  PFN_vkCmdDrawIndirect           vkCmdDrawIndirect           = nullptr;
  PFN_vkCmdDrawIndexedIndirect    vkCmdDrawIndexedIndirect    = nullptr;
  PFN_vkCmdDispatch               vkCmdDispatch               = nullptr;
  PFN_vkCmdDispatchIndirect       vkCmdDispatchIndirect       = nullptr;
  PFN_vkCmdCopyBuffer             vkCmdCopyBuffer             = nullptr;
  PFN_vkCmdCopyImage              vkCmdCopyImage              = nullptr;
  PFN_vkCmdBlitImage              vkCmdBlitImage              = nullptr;
  PFN_vkCmdCopyBufferToImage      vkCmdCopyBufferToImage      = nullptr;
  PFN_vkCmdCopyImageToBuffer      vkCmdCopyImageToBuffer      = nullptr;
  PFN_vkCmdUpdateBuffer           vkCmdUpdateBuffer           = nullptr;
  PFN_vkCmdFillBuffer             vkCmdFillBuffer             = nullptr;
  PFN_vkCmdClearColorImage        vkCmdClearColorImage        = nullptr;
  PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage = nullptr;
  PFN_vkCmdClearAttachments       vkCmdClearAttachments       = nullptr;
  PFN_vkCmdResolveImage           vkCmdResolveImage           = nullptr;
  PFN_vkCmdSetEvent               vkCmdSetEvent               = nullptr;
  PFN_vkCmdResetEvent             vkCmdResetEvent             = nullptr;
  PFN_vkCmdWaitEvents             vkCmdWaitEvents             = nullptr;
  PFN_vkCmdPipelineBarrier        vkCmdPipelineBarrier        = nullptr;
  PFN_vkCmdBeginQuery             vkCmdBeginQuery             = nullptr;
  PFN_vkCmdEndQuery               vkCmdEndQuery               = nullptr;
  PFN_vkCmdResetQueryPool         vkCmdResetQueryPool         = nullptr;
  PFN_vkCmdWriteTimestamp         vkCmdWriteTimestamp         = nullptr;
  PFN_vkCmdCopyQueryPoolResults   vkCmdCopyQueryPoolResults   = nullptr;
  PFN_vkCmdPushConstants          vkCmdPushConstants          = nullptr;
  PFN_vkCmdBeginRenderPass        vkCmdBeginRenderPass        = nullptr;
  PFN_vkCmdNextSubpass            vkCmdNextSubpass            = nullptr;
  PFN_vkCmdEndRenderPass          vkCmdEndRenderPass          = nullptr;
  PFN_vkCmdExecuteCommands        vkCmdExecuteCommands        = nullptr;

  // VK_KHR_swapchain
  PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = nullptr;
  PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = nullptr;
  PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = nullptr;
  PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = nullptr;
  PFN_vkQueuePresentKHR vkQueuePresentKHR = nullptr;
  PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR = nullptr;
  PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR = nullptr;
  PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR = nullptr;
  PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR = nullptr;
  // VK_KHR_display_swapchain
  PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR = nullptr;
  // VK_KHR_device_group
  PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR = nullptr;
  PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR = nullptr;
  PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR = nullptr;
  // VK_KHR_maintenance1
  PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR = nullptr;
  // VK_KHR_external_memory_fd
  PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR = nullptr;
  PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR = nullptr;
  // VK_KHR_external_semaphore_fd
  PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR = nullptr;
  PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR = nullptr;
  // VK_KHR_push_descriptor
  PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR = nullptr;
  PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR = nullptr;
  // VK_KHR_descriptor_update_template
  PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR = nullptr;
  PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR = nullptr;
  PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR = nullptr;
  // VK_KHR_shared_presentable_image
  PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR = nullptr;
  // VK_KHR_external_fence_fd
  PFN_vkImportFenceFdKHR vkImportFenceFdKHR = nullptr;
  PFN_vkGetFenceFdKHR vkGetFenceFdKHR = nullptr;
  // VK_KHR_get_memory_requirements2
  PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR = nullptr;
  PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR = nullptr;
  PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR = nullptr;
  // VK_KHR_sampler_ycbcr_conversion
  PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR = nullptr;
  PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR = nullptr;
  // VK_KHR_bind_memory2
  PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR = nullptr;
  PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR = nullptr;
  // VK_KHR_maintenance3
  PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR = nullptr;
  // VK_EXT_debug_marker
  PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT = nullptr;
  PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT = nullptr;
  PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT = nullptr;
  PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT = nullptr;
  PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT = nullptr;
  // VK_EXT_display_control
  PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT = nullptr;
  PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT = nullptr;
  PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT = nullptr;
  PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT = nullptr;
  // VK_EXT_discard_rectangles
  PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT = nullptr;
  // VK_EXT_hdr_metadata
  PFN_vkSetHdrMetadataEXT vkSetHdrMetadata = nullptr;
  // VK_EXT_sample_locations
  PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT = nullptr;
  PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT = nullptr;
  // VK_EXT_validation_cache
  PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT = nullptr;
  PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT = nullptr;
  PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT = nullptr;
  PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT = nullptr;
  // VK_EXT_external_memory_host
  PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT = nullptr;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_VULKAN_H
