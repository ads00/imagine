/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_VK_SURFACE_H
#define IG_ENVI_VK_SURFACE_H

#include "imagine/envi/widget.h"
#include "imagine/envi/impl_vk/detail/physical.h"

namespace ig {
namespace vk {

enum class present_mode { immediate = 0, mailbox = 1, fifo = 2, fifo_relaxed = 3 };

class display;
class IG_API surface : public managed<VkSurfaceKHR_T*> {
public:
  friend class swapchain;

  explicit surface(const physical& physical, const widget& widget);
  virtual ~surface();

  int32_t get_queue() const;
  // vulkan
  auto get_capabilities() const -> surface_capabilities;

  const physical& phys;
  const instance& inst;
  const widget&   widg;

protected:
  auto support_format(format fmt) const -> format;
  auto support_present_mode(present_mode present) const -> present_mode;

  void impl_get();

private:
  struct impl;
  std::unique_ptr<impl> impl_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_SURFACE_H
