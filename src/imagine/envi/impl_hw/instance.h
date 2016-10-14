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

#ifndef IG_ENVI_INSTANCE_H
#define IG_ENVI_INSTANCE_H

#include "imagine/envi/impl_hw/vulkan.h"

#include <vector>
#include <unordered_map>

namespace ig {

IG_API VkBool32 VKAPI_CALL vk_dbg_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT type,
                                           uint64_t obj, size_t location, int32_t code, const char* prefix, const char* msg, void* udata);

class IG_API instance : public VKObject<VkInstance>, public VKpfn {
public:
  friend vulkan;
  friend class physical;

  explicit instance(bool validation = false);
  virtual ~instance();

  bool dbg(decltype(vk_dbg_callback) callback = vk_dbg_callback);

  bool supported(const std::string& name) const;

  auto& extensions() const { return extensions_; }
  auto& layers() const     { return layers_; }

private:
  void configure();
  void acquire_physicals();

private:
  std::vector<VkExtensionProperties> extensions_;
  std::vector<VkLayerProperties>     layers_;
  std::unordered_map<VkPhysicalDevice, VkPhysicalDeviceProperties> physicals_;

  bool validation_;
  VkDebugReportCallbackEXT dbg_callback_;
};

} // namespace ig

#endif // IG_ENVI_INSTANCE_H
