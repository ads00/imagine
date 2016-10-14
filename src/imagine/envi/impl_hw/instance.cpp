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

#include "imagine/envi/impl_hw/instance.h"
#include "imagine/core/log.h"

namespace ig {

static std::string ig_engine = "imagine_vk";

VkBool32 vk_dbg_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT type,
                         uint64_t obj, size_t location, int32_t code, const char* prefix, const char* msg, void* udata) {
  LOG(info) << "(Vk - " << prefix << "): " << msg;
  return false;
}

instance::instance(bool validation)
  : validation_{validation}
  , dbg_callback_{nullptr} {

  vulkan::discover();
  VkApplicationInfo app_info = {
    VK_STRUCTURE_TYPE_APPLICATION_INFO, // VkStructureType    sType;
    nullptr,                            // const void*        pNext;
    nullptr,                            // const char*        pApplicationName;
    1,                                  // uint32_t           applicationVersion;
    ig_engine.c_str(),                  // const char*        pEngineName;
    1,                                  // uint32_t           engineVersion;
    VK_API_VERSION_1_0                  // uint32_t           apiVersion
  };

  configure();
  std::vector<const char*> extensions(extensions_.size());
  std::transform(extensions_.begin(), extensions_.end(), extensions.begin(), [](auto& extension)
  { return extension.extensionName; });
  std::vector<const char*> layers(layers_.size());
  std::transform(layers_.begin(), layers_.end(), layers.begin(), [](auto& layer)
  { return layer.layerName; });

  VkInstanceCreateInfo inst_info = {
    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,   // VkStructureType             sType;
    nullptr,                                  // const void*                 pNext;
    0,                                        // VkInstanceCreateFlags       flags;
    &app_info,                                // const VkApplicationInfo*    pApplicationInfo;
    static_cast<uint32_t>(layers.size()),     // uint32_t                    enabledLayerCount;
    layers.data(),                            // const char* const*          ppEnabledLayerNames;
    static_cast<uint32_t>(extensions.size()), // uint32_t                    enabledExtensionCount;
    extensions.data()                         // const char* const*          ppEnabledExtensionNames;
  };

  auto res = vkCreateInstance(&inst_info, nullptr, &h_);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create instance : " + vulkan::to_string(res)};
  }

  vulkan::acquire(*this);
  acquire_physicals();
}

instance::~instance() {
  if (dbg_callback_) {
    vkDestroyDebugReportCallbackEXT(h_, dbg_callback_, nullptr);
    dbg_callback_ = nullptr;
  }
  assert(h_ && "Invalid instance");
  vkDestroyInstance(h_, nullptr);
}

bool instance::dbg(decltype(vk_dbg_callback) callback) {
  if (!validation_ || !supported(VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
    return false;

  VkDebugReportCallbackCreateInfoEXT debugreport_info = {
    VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT, // VkStructureType                 sType;
    nullptr,                                                 // const void*                     pNext;
    VK_DEBUG_REPORT_WARNING_BIT_EXT               |
    VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT   |
    VK_DEBUG_REPORT_ERROR_BIT_EXT,                           // VkDebugReportFlagsEXT           flags;
    callback,                                                // PFN_vkDebugReportCallbackEXT    pfnCallback;
    reinterpret_cast<void*>(this)                            // void*                           pUserData;
  };

  return vkCreateDebugReportCallbackEXT(h_, &debugreport_info, nullptr, &dbg_callback_) == VK_SUCCESS;
}

bool instance::supported(const std::string& name) const {
  return std::find_if(extensions_.begin(), extensions_.end(), [&name](auto& extension) {
    return std::string{extension.extensionName} == name;
  }) != extensions_.end();
}

void instance::configure() {
  // Extensions
  uint32_t extension_count = 0;
  assert(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr) == VK_SUCCESS
         && "Failed to enumerate instance extension properties");

  if (extension_count > 0) {
    extensions_.resize(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions_.data());
  }

  if (validation_) {
    // Layers
    uint32_t layer_count = 0;
    assert(vkEnumerateInstanceLayerProperties(&layer_count, nullptr) == VK_SUCCESS
           && "Failed to enumerate instance layer properties");

    if (layer_count > 0) {
      layers_.resize(layer_count);
      vkEnumerateInstanceLayerProperties(&layer_count, layers_.data());

      layers_.erase(std::remove_if(layers_.begin(), layers_.end(), [](auto& layer) {
        return std::string{layer.layerName} != "VK_LAYER_LUNARG_standard_validation";
      }), layers_.end());
    }
  }
}

void instance::acquire_physicals() {
  // Physical devices
  uint32_t physical_devices_count = 1;
  assert(vkEnumeratePhysicalDevices(h_, &physical_devices_count, nullptr) == VK_SUCCESS
         && "Failed to enumerate physical devices");

  if (physical_devices_count > 0) {
    std::vector<VkPhysicalDevice> physical_devices(physical_devices_count);
    vkEnumeratePhysicalDevices(h_, &physical_devices_count, physical_devices.data());

    for (auto& physical_device : physical_devices)
      vkGetPhysicalDeviceProperties(physical_device, &physicals_[physical_device]);
  }
}

} // namespace ig
