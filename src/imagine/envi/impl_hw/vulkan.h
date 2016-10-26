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

#ifndef IG_ENVI_API_H
#define IG_ENVI_API_H

#include "imagine/core/utility.h"
#include "imagine/core/flags.h"
#include "imagine/envi/library.h"

#if defined(IG_WIN)
 #define VK_DYN_LOADER "vulkan-1.dll"
 #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(IG_UNIX)
 #define VK_DYN_LOADER "libvulkan.so.1"
 #define VK_USE_PLATFORM_XCB_KHR
#endif
#define  VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

#include <utility>
#include <algorithm>

namespace ig {

class IG_API vulkan {
public:
  friend class instance;
  friend class device;

  vulkan() = delete;
  static void discover();

  static std::string to_string(VkResult res);

private:
  static void acquire(instance& i);
  static void acquire(device& d);

  static library vulkan_;
};

template <typename T>
class VKObject {
public:
  VKObject() = default;
  constexpr explicit VKObject(T&& h) : h_{std::move(h)} {}

  virtual ~VKObject() {
    h_ = nullptr; 
  }

  operator T*() { return &h_; }
  constexpr operator const T*() const { return &h_; }
  constexpr operator const T()  const { return h_; }

  VKObject(const VKObject&) = delete;
  VKObject& operator=(const VKObject&) = delete;

  T h_ = nullptr;
};

enum class pipeline_stage : uint32_t {
  top_of_pipe   = 0x00001, draw_indirect = 0x00002,
  vertex_input  = 0x00004, 
  vertex_shader = 0x00008,

  tesselation_control_shader = 0x00010, tesselation_evaluation_shader = 0x00020,
  geometry_shader            = 0x00040, fragment_shader               = 0x00080,
  early_fragment_tests       = 0x00100, late_fragment_tests           = 0x00200,
  color_attachment_output    = 0x00400,
  compute_shader             = 0x00800,
  transfer                   = 0x01000,
  bottom_of_pipe             = 0x02000,

  host = 0x04000, graphics = 0x08000, commands = 0x10000
}; using pipeline_stages = flags<pipeline_stage>;

} // namespace ig

// VK_core
extern IG_API PFN_vkGetInstanceProcAddr                  vkGetInstanceProcAddr;
extern IG_API PFN_vkCreateInstance                       vkCreateInstance;
extern IG_API PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern IG_API PFN_vkEnumerateInstanceLayerProperties     vkEnumerateInstanceLayerProperties;

struct IG_API VKpfn {
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
  PFN_vkDestroySurfaceKHR                       vkDestroySurfaceKHR                       = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceSupportKHR      vkGetPhysicalDeviceSurfaceSupportKHR      = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceFormatsKHR      vkGetPhysicalDeviceSurfaceFormatsKHR      = nullptr;
  PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;

  // VK_KHR_display
  PFN_vkGetPhysicalDeviceDisplayPropertiesKHR      vkGetPhysicalDeviceDisplayPropertiesKHR      = nullptr;
  PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR = nullptr;
  PFN_vkGetDisplayPlaneSupportedDisplaysKHR        vkGetDisplayPlaneSupportedDisplaysKHR        = nullptr;
  PFN_vkGetDisplayModePropertiesKHR                vkGetDisplayModePropertiesKHR                = nullptr;
  PFN_vkCreateDisplayModeKHR                       vkCreateDisplayModeKHR                       = nullptr;
  PFN_vkGetDisplayPlaneCapabilitiesKHR             vkGetDisplayPlaneCapabilitiesKHR             = nullptr;
  PFN_vkCreateDisplayPlaneSurfaceKHR               vkCreateDisplayPlaneSurfaceKHR               = nullptr;

  #if defined(VK_USE_PLATFORM_XLIB_KHR)
  // VK_KHR_xlib_surface
  PFN_vkCreateXlibSurfaceKHR                        vkCreateXlibSurfaceKHR                        = nullptr;
  PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_XCB_KHR)
  // VK_KHR_xcb_surface
  PFN_vkCreateXcbSurfaceKHR                        vkCreateXcbSurfaceKHR                        = nullptr;
  PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_WAYLAND_KHR)
  // VK_KHR_wayland_surface
  PFN_vkCreateWaylandSurfaceKHR                        vkCreateWaylandSurfaceKHR                        = nullptr;
  PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_MIR_KHR)
  // VK_KHR_mir_surface
  PFN_vkCreateMirSurfaceKHR                        vkCreateMirSurfaceKHR                        = nullptr;
  PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR = nullptr;
  #endif
  #if defined(VK_USE_PLATFORM_ANDROID_KHR)
  // VK_KHR_android_surface
  PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR = nullptr;
  #endif

  #if defined(VK_USE_PLATFORM_WIN32_KHR)
  // VK_KHR_win32_surface
  PFN_vkCreateWin32SurfaceKHR                        vkCreateWin32SurfaceKHR                        = nullptr;
  PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
  #endif

  // VK_EXT_debug_report
  PFN_vkCreateDebugReportCallbackEXT  vkCreateDebugReportCallbackEXT  = nullptr;
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;
  PFN_vkDebugReportMessageEXT         vkDebugReportMessageEXT         = nullptr;
};

struct IG_API VKpfnd {
  // VK_device_core
  PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;

  PFN_vkDestroyDevice                    vkDestroyDevice                    = nullptr;
  PFN_vkGetDeviceQueue                   vkGetDeviceQueue                   = nullptr;
  PFN_vkQueueSubmit                      vkQueueSubmit                      = nullptr;
  PFN_vkQueueWaitIdle                    vkQueueWaitIdle                    = nullptr;
  PFN_vkDeviceWaitIdle                   vkDeviceWaitIdle                   = nullptr;
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
  PFN_vkCreateFence                      vkCreateFence                      = nullptr;
  PFN_vkDestroyFence                     vkDestroyFence                     = nullptr;
  PFN_vkResetFences                      vkResetFences                      = nullptr;
  PFN_vkGetFenceStatus                   vkGetFenceStatus                   = nullptr;
  PFN_vkWaitForFences                    vkWaitForFences                    = nullptr;
  PFN_vkCreateSemaphore                  vkCreateSemaphore                  = nullptr;
  PFN_vkDestroySemaphore                 vkDestroySemaphore                 = nullptr;
  PFN_vkCreateEvent                      vkCreateEvent                      = nullptr;
  PFN_vkDestroyEvent                     vkDestroyEvent                     = nullptr;
  PFN_vkGetEventStatus                   vkGetEventStatus                   = nullptr;
  PFN_vkSetEvent                         vkSetEvent                         = nullptr;
  PFN_vkResetEvent                       vkResetEvent                       = nullptr;
  PFN_vkCreateQueryPool                  vkCreateQueryPool                  = nullptr;
  PFN_vkDestroyQueryPool                 vkDestroyQueryPool                 = nullptr;
  PFN_vkGetQueryPoolResults              vkGetQueryPoolResults              = nullptr;
  PFN_vkCreateBuffer                     vkCreateBuffer                     = nullptr;
  PFN_vkDestroyBuffer                    vkDestroyBuffer                    = nullptr;
  PFN_vkCreateBufferView                 vkCreateBufferView                 = nullptr;
  PFN_vkDestroyBufferView                vkDestroyBufferView                = nullptr;
  PFN_vkCreateImage                      vkCreateImage                      = nullptr;
  PFN_vkDestroyImage                     vkDestroyImage                     = nullptr;
  PFN_vkGetImageSubresourceLayout        vkGetImageSubresourceLayout        = nullptr;
  PFN_vkCreateImageView                  vkCreateImageView                  = nullptr;
  PFN_vkDestroyImageView                 vkDestroyImageView                 = nullptr;
  PFN_vkCreateShaderModule               vkCreateShaderModule               = nullptr;
  PFN_vkDestroyShaderModule              vkDestroyShaderModule              = nullptr;
  PFN_vkCreatePipelineCache              vkCreatePipelineCache              = nullptr;
  PFN_vkDestroyPipelineCache             vkDestroyPipelineCache             = nullptr;
  PFN_vkGetPipelineCacheData             vkGetPipelineCacheData             = nullptr;
  PFN_vkMergePipelineCaches              vkMergePipelineCaches              = nullptr;
  PFN_vkCreateGraphicsPipelines          vkCreateGraphicsPipelines          = nullptr;
  PFN_vkCreateComputePipelines           vkCreateComputePipelines           = nullptr;
  PFN_vkDestroyPipeline                  vkDestroyPipeline                  = nullptr;
  PFN_vkCreatePipelineLayout             vkCreatePipelineLayout             = nullptr;
  PFN_vkDestroyPipelineLayout            vkDestroyPipelineLayout            = nullptr;
  PFN_vkCreateSampler                    vkCreateSampler                    = nullptr;
  PFN_vkDestroySampler                   vkDestroySampler                   = nullptr;
  PFN_vkCreateDescriptorSetLayout        vkCreateDescriptorSetLayout        = nullptr;
  PFN_vkDestroyDescriptorSetLayout       vkDestroyDescriptorSetLayout       = nullptr;
  PFN_vkCreateDescriptorPool             vkCreateDescriptorPool             = nullptr;
  PFN_vkDestroyDescriptorPool            vkDestroyDescriptorPool            = nullptr;
  PFN_vkResetDescriptorPool              vkResetDescriptorPool              = nullptr;
  PFN_vkAllocateDescriptorSets           vkAllocateDescriptorSets           = nullptr;
  PFN_vkFreeDescriptorSets               vkFreeDescriptorSets               = nullptr;
  PFN_vkUpdateDescriptorSets             vkUpdateDescriptorSets             = nullptr;
  PFN_vkCreateFramebuffer                vkCreateFramebuffer                = nullptr;
  PFN_vkDestroyFramebuffer               vkDestroyFramebuffer               = nullptr;
  PFN_vkCreateRenderPass                 vkCreateRenderPass                 = nullptr;
  PFN_vkDestroyRenderPass                vkDestroyRenderPass                = nullptr;
  PFN_vkGetRenderAreaGranularity         vkGetRenderAreaGranularity         = nullptr;
  PFN_vkCreateCommandPool                vkCreateCommandPool                = nullptr;
  PFN_vkDestroyCommandPool               vkDestroyCommandPool               = nullptr;
  PFN_vkResetCommandPool                 vkResetCommandPool                 = nullptr;
  PFN_vkAllocateCommandBuffers           vkAllocateCommandBuffers           = nullptr;
  PFN_vkFreeCommandBuffers               vkFreeCommandBuffers               = nullptr;
  PFN_vkBeginCommandBuffer               vkBeginCommandBuffer               = nullptr;
  PFN_vkEndCommandBuffer                 vkEndCommandBuffer                 = nullptr;
  PFN_vkResetCommandBuffer               vkResetCommandBuffer               = nullptr;

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
  PFN_vkCreateSwapchainKHR    vkCreateSwapchainKHR    = nullptr;
  PFN_vkDestroySwapchainKHR   vkDestroySwapchainKHR   = nullptr;
  PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = nullptr;
  PFN_vkAcquireNextImageKHR   vkAcquireNextImageKHR   = nullptr;
  PFN_vkQueuePresentKHR       vkQueuePresentKHR       = nullptr;

  // VK_KHR_display_swapchain
  PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR = nullptr;

  // VK_EXT_debug_marker
  PFN_vkDebugMarkerSetObjectTagEXT  vkDebugMarkerSetObjectTagEXT  = nullptr;
  PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT = nullptr;
  PFN_vkCmdDebugMarkerBeginEXT      vkCmdDebugMarkerBeginEXT      = nullptr;
  PFN_vkCmdDebugMarkerEndEXT        vkCmdDebugMarkerEndEXT        = nullptr;
  PFN_vkCmdDebugMarkerInsertEXT     vkCmdDebugMarkerInsertEXT     = nullptr;
};

#endif // IG_ENVI_API_H