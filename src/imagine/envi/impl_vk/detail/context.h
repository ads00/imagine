/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_VK_CONTEXT_H
#define IG_ENVI_VK_CONTEXT_H

#include "imagine/envi/impl_vk/detail/instance.h"
#include "imagine/envi/impl_vk/detail/physical.h"
#include "imagine/envi/impl_vk/detail/device.h"
#include "imagine/envi/impl_vk/queue.h"

namespace ig {
namespace vk {

class context : public instance {
public:
  explicit context(hardware unit, bool validation = false)
    : instance{validation}
    , unit_{unit} {}

  template <typename Id>
  auto build(std::initializer_list<capabilities> caps, Id id = 0, bool threadded = false);

  auto queue_line(uint32_t i = 0) const { return std::make_unique<queue>(*device_, i); }
  void print_infos() const;

private:
  hardware unit_;

  std::shared_ptr<physical>
  physical_;
  std::unique_ptr<device>
  device_;
};

template <typename Id>
auto context::build(std::initializer_list<capabilities> caps, Id id, bool threadded) {
  assert(physicals_.size() && "Vulkan not supported on this machine (no physical device available)");

  auto it =
    std::find_if(
      physicals_.begin(),
      physicals_.end(),
      [this, &id](auto& physical) { return physical->get_type() == unit_ && !id--; });

  physical_ = it != physicals_.end()
    ? *it
    : *physicals_.begin();
  device_ = std::make_unique<device>(*physical_, std::vector<capabilities>{caps});

  print_infos();
  /*
  if (threaded) {

  } else {

  }
  */

  return
  std::make_tuple(
    physical_,
    std::ref(device_));
}

void context::print_infos() const {}

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_CONTEXT_H
