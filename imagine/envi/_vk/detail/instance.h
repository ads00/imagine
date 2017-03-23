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

#ifndef IG_ENVI_VK_INSTANCE_H
#define IG_ENVI_VK_INSTANCE_H

#include "imagine/envi/_vk/detail/wrapper.h"

#include <algorithm>
#include <tuple>
#include <vector>

namespace ig {
namespace vk {

enum class hardware;
enum class queue_capability : uint32_t; using queue_capabilities = flags<queue_capability>;
enum class memory_property  : uint32_t; using memory_properties  = flags<memory_property>;

class iPfn;
class physical;
class ig_api instance : public managed<VkInstance_T*> {
public:
  friend vulkan;
  friend device;

  explicit instance(bool validation = false);
  virtual ~instance();

  bool dbg();
  bool supported(const std::string& name) const;

  template <size_t... Is>
  auto gen_context_queues(std::unique_ptr<device>& devi, std::index_sequence<Is...>) const {
    return std::make_tuple(
      std::move([&devi](uint32_t i) { return std::make_unique<queue>(*devi, i); }(Is))...);
  }

  template <int32_t q, int32_t p = 0>
  auto make_context(hardware unit, const std::vector<queue_capabilities>& rq) const {
    auto id = p;
    auto it = std::find_if(physicals_.begin(), physicals_.end(), [&unit, &id](auto& phys) {
      return phys->get_type() == unit && !id--;
    });
    auto phys = it != physicals_.end()
      ? *it
      : *physicals_.begin();

    auto devi = std::make_unique<device>(*phys, rq);
    return std::tuple_cat(std::make_tuple(phys, std::move(devi)), std::move(gen_context_queues(devi, std::make_index_sequence<q>{})));
  }

  auto& operator->() const { return ipfn_; }

private:
  virtual void pre_acquire() override;
  virtual void post_acquire() override;

private:
  bool validation_;
  std::vector< std::shared_ptr<physical> > 
    physicals_;

  struct impl; 
  std::unique_ptr<impl> impl_;
  std::unique_ptr<iPfn> ipfn_;
};

enum class hardware {
  other = 0,
  integrated = 1, discrete = 2, virtualized = 3, cpu = 4 };

enum class queue_capability : uint32_t {
  graphics = 0x001, compute = 0x002,
  transfer = 0x004, sparse  = 0x008, present = 0x010, universal = graphics | compute | transfer };

enum class memory_property : uint32_t {
  device_local = 0x001,
  host_visible = 0x002, host_coherent = 0x004, host_cached = 0x008, lazily_allocated = 0x010 };

enum class format {
  r8_unorm       = 9,  r8_snorm       = 10,  r8_srgb      = 15,
  r8g8_unorm     = 16, r8g8_snorm     = 17,  r8g8_srgb    = 22,
  r8g8b8_unorm   = 23, r8g8b8_snorm   = 24, r8g8b8_srgb   = 29,
  b8g8r8_unorm   = 30, b8g8r8_snorm   = 31, b8g8r8_srgb   = 36,
  r8g8b8a8_unorm = 37, r8g8b8a8_snorm = 38, r8g8b8a8_srgb = 43,
  b8g8r8a8_unorm = 44, b8g8r8a8_snorm = 45, b8g8r8a8_srgb = 50,

  r16_unorm          = 70, r16_snorm          = 71, r16_sfloat          = 76,
  r16g16_unorm       = 77, r16g16_snorm       = 78, r16g16_sfloat       = 83,
  r16g16b16_unorm    = 84, r16g16b16_snorm    = 85, r16g16b16_sfloat    = 90,
  r16g16b16a16_unorm = 91, r16g16b16a16_snorm = 92, r16g16b16a16_sfloat = 97,

  r32_sfloat = 100, r32g32_sfloat = 103, r32g32b32_sfloat = 106, r32g32b32a32_sfloat = 109,
  r64_sfloat = 112, r64g64_sfloat = 115, r64g64b64_sfloat = 118, r64g64b64a64_sfloat = 121,

  d16_unorm  = 124,
  d32_sfloat = 126, d16_unorm_s8_uint = 128, d24_unorm_s8_uint = 129, d32_sfloat_s8_uint = 130 };

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_INSTANCE_H
