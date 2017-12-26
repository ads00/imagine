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

  auto build(
    std::initializer_list<capabilities> caps,
    uint32_t id = 0,
    bool thread = false);

  auto queue_line(uint32_t i = 0) const { return std::make_unique<queue>(*device_, i); }

private:
  hardware unit_;
  std::shared_ptr<physical> physical_;
  std::unique_ptr<device>   device_;
};

auto context::build(std::initializer_list<capabilities> caps, uint32_t id, bool thread) {
  auto it = std::find_if(
    physicals_.begin(),
    physicals_.end(),
    [this, &id](auto& phys) { return phys->get_type() == unit_ && !id--; });

  physical_ = it != physicals_.end()
    ? *it
    : *physicals_.begin();
  device_ = std::make_unique<device>(*physical_, std::vector<capabilities>{caps});
  return std::make_pair(
    physical_,
    std::ref(device_));
}

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_CONTEXT_H
