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

#include "imagine/envi/impl_hw/physical.h"

namespace ig {

physical::physical(const instance& instance, type_t type, uint32_t i)
  : inst{instance} {

  auto it = std::find_if(inst.physicals_.begin(), inst.physicals_.end(), [&type, &i](auto& phys) {
    return phys.second.deviceType == type && !i--;
  });

  std::tie(h_, properties_) = it != inst.physicals_.end() 
    ? *it 
    : *inst.physicals_.begin();
  configure();
}

void physical::configure() {
  inst.vkGetPhysicalDeviceFeatures(h_, &features_);

  // Queue families
  uint32_t family_count = 0;
  inst.vkGetPhysicalDeviceQueueFamilyProperties(h_, &family_count, nullptr);

  if (family_count > 0) {
    queues_families_.resize(family_count);
    inst.vkGetPhysicalDeviceQueueFamilyProperties(h_, &family_count, queues_families_.data());
  }
}

} // namespace ig
