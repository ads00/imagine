/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_vk/detail/vulkan.h"
#include "imagine/envi/impl_vk/detail/instance.h"
#include "imagine/envi/impl_vk/detail/physical.h"
#include "imagine/core/log.h"

namespace ig {
namespace vk {

struct instance::impl {
  // Callback on warnings / errors
  // Require VK_EXT_debug_report rev. > 3 and validation enabled
  VkDebugReportCallbackEXT dbg_callback;

  // Vulkan instance extensions
  // Registered and published KHR and EXT extensions supported
  // VK_KHR_surface rev. 25 [James Jones, Ian Elliott]
  // VK_KHR_display rev. 21 [James Jones, Norbert Nopper]
  // VK_KHR_get_physical_device_properties2 rev. 1 [Jeff Bolz]
  // VK_KHR_device_group_creation rev. 1 [Jeff Bolz]
  // VK_KHR_external_memory_capabilities rev. 1 [James Jones]
  // VK_KHR_external_semaphore_capabilities rev. 1 [James Jones]
  // VK_KHR_external_fence_capabilities rev. 1 [Jesse Hall]
  // VK_KHR_get_surface_capabilities2 rev. 1 [James Jones]
  // VK_EXT_debug_report rev. 9 [Courtney Goeltzenleuchter]
  // VK_EXT_validation_flags rev. 1 [Tobin Ehlis]
  // VK_EXT_direct_mode_display rev. 1 [James Jones]
  // VK_EXT_display_surface_counter rev. 1 [James Jones]
  // VK_EXT_swapchain_colorspace rev. 3 [Courtney Goeltzenleuchter]
  // VK_EXT_debug_utils rev. 1 [Mark Young]

  // VK_KHR_xlib_surface rev. 6 [Jesse Hall, Ian Elliott]
  // VK_KHR_xcb_surface rev. 6 [Jesse Hall, Ian Elliott]
  // VK_KHR_wayland_surface rev. 6 [Jesse Hall, Ian Elliott]
  // VK_KHR_mir_surface rev. 4 [Jesse Hall, Ian Elliott]
  // VK_KHR_win32_surface rev. 6 [Jesse Hall, Ian Elliott]
  // VK_KHR_android_surface rev. 6 [Jesse Hall]
  // VK_MVK_macos_surface rev. 2 [Bill Hollings]
  // VK_MVK_ios_surface rev. 2 [Bill Hollings]
  // VK_NN_vi_surface rev. 1 [Mathias Heyer]
  std::vector<VkExtensionProperties> extensions;

  // Official Vulkan SDK layers
  // Standard validation meta-layer (VK_LAYER_LUNARG_standard_validation)
  // VK_LAYER_GOOGLE_threading impl. 1
  // VK_LAYER_LUNARG_parameter_validation impl. 1
  // VK_LAYER_LUNARG_object_tracker impl. 1
  // VK_LAYER_LUNARG_core_validation impl. 1
  // VK_LAYER_GOOGLE_unique_objects impl. 1
  std::vector<VkLayerProperties> layers; };

static std::string ig_engine =
    "imagine_vk";
static std::vector<std::string> meta_layers = {
    "VK_LAYER_GOOGLE_threading",
    "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_object_tracker",
    "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_GOOGLE_unique_objects" };

VkBool32 vk_dbg_callback(
  VkDebugReportFlagsEXT flags,
  VkDebugReportObjectTypeEXT type,
  uint64_t obj, size_t location, int32_t code, const char* prefix, const char* msg, void* udata) {
  log_(
    info,
    "(Vk - {}): {}",
    prefix,
    msg);
  return false;
}

instance::instance(bool validation)
  : validation_{validation}
  , impl_{std::make_unique<impl>()}
  , ipfn_{std::make_unique<iPfn>()} {

  vulkan::discover();
  VkApplicationInfo app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.applicationVersion = 1;
    app_info.engineVersion      = 1;
    app_info.pEngineName        = ig_engine.c_str();
      app_info.apiVersion = VK_API_VERSION_1_0;

  impl_get();
  std::vector<const char*> extensions(impl_->extensions.size());
  std::transform(
    impl_->extensions.begin(),
    impl_->extensions.end(),
    extensions.begin(),
    [](auto& extension) { return extension.extensionName; });
  std::vector<const char*> layers(impl_->layers.size());
  std::transform(
    impl_->layers.begin(),
    impl_->layers.end(),
    layers.begin(),
    [](auto& layer) { return layer.layerName; });

  VkInstanceCreateInfo inst_info {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledLayerCount     = static_cast<uint32_t>(layers.size());
      inst_info.ppEnabledLayerNames = layers.data();
    inst_info.enabledExtensionCount     = static_cast<uint32_t>(extensions.size());
      inst_info.ppEnabledExtensionNames = extensions.data();

  auto res =
    vkCreateInstance(
      &inst_info,
      nullptr,
      &handle);
  if (res != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create instance : " + vulkan::to_string(res)};
  } else {
    vulkan::acquire_fn(*this);
    physicals_get();
  }
}

instance::~instance() {
  if (impl_->dbg_callback)
    ipfn_->vkDestroyDebugReportCallbackEXT(
      handle,
      impl_->dbg_callback,
      nullptr);
  ipfn_->vkDestroyInstance(
    handle,
    nullptr);
}

bool instance::dbg() {
  if (!validation_ || !is_supported(VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
    return false;

  VkDebugReportCallbackCreateInfoEXT debugreport_info {};
    debugreport_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debugreport_info.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT             |
                             VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                             VK_DEBUG_REPORT_ERROR_BIT_EXT;
    debugreport_info.pfnCallback = vk_dbg_callback;
    debugreport_info.pUserData   = reinterpret_cast<void*>(this);

  return ipfn_->vkCreateDebugReportCallbackEXT(
    handle,
    &debugreport_info,
    nullptr,
    &impl_->dbg_callback) == VK_SUCCESS;
}

bool instance::is_supported(const std::string& name) const {
  return std::find_if(
    impl_->extensions.begin(),
    impl_->extensions.end(),
    [&name](auto& ext) { return std::string{ext.extensionName} == name; }) != impl_->extensions.end();
}

void instance::impl_get() {
  // Extensions
  uint32_t extension_count = 0;
  assert(
    vkEnumerateInstanceExtensionProperties(
      nullptr,
      &extension_count,
      nullptr) == VK_SUCCESS
    && "Failed to enumerate instance extension properties");

  if (extension_count > 0) {
    impl_->extensions.resize(extension_count);
    vkEnumerateInstanceExtensionProperties(
      nullptr,
      &extension_count,
      impl_->extensions.data());
  }

  if (validation_) {
    // Layers
    uint32_t layer_count = 0;
    assert(
      vkEnumerateInstanceLayerProperties(
        &layer_count,
        nullptr) == VK_SUCCESS
      && "Failed to enumerate instance layer properties");

    if (layer_count > 0) {
      impl_->layers.resize(layer_count);
      vkEnumerateInstanceLayerProperties(
        &layer_count,
        impl_->layers.data());

      impl_->layers.erase(
        std::remove_if(
          impl_->layers.begin(),
          impl_->layers.end(),
          [](auto& layer) { return std::find(meta_layers.begin(), meta_layers.end(), std::string{layer.layerName}) == meta_layers.end();
      }), impl_->layers.end());
    }
  }
}

void instance::physicals_get() {
  // Physical devices
  uint32_t physical_devices_count = 1;
  ipfn_->vkEnumeratePhysicalDevices(
    handle,
    &physical_devices_count,
    nullptr);

  if (physical_devices_count > 0) {
    std::vector<VkPhysicalDevice> physical_devices(physical_devices_count);
    ipfn_->vkEnumeratePhysicalDevices(
      handle,
      &physical_devices_count,
      physical_devices.data());

    for (auto& physical_device : physical_devices)
      physicals_.emplace_back(std::make_shared<physical>(*this, physical_device));
  }
}

} // namespace vk
} // namespace ig
