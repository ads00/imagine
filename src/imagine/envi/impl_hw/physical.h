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

#ifndef IG_ENVI_PHYSICAL_H
#define IG_ENVI_PHYSICAL_H

#include "imagine/envi/impl_hw/instance.h"

#include <vector>

namespace ig {

class IG_API physical : public VKObject<VkPhysicalDevice> {
public:
  friend device;
  friend class surface;

  enum type_t {
    other = 0,
    integrated  = 1, discrete = 2,
    virtualized = 3, cpu      = 4 };
  
  explicit physical(const instance& instance, type_t type, uint32_t i = 0);

  auto& properties() const { return properties_; }
  auto& features() const   { return features_; }

  const instance& inst;

private:
  void configure();

private:
  VkPhysicalDeviceProperties properties_; 
  VkPhysicalDeviceFeatures   features_;
  std::vector<VkQueueFamilyProperties> queues_families_;
};

} // namespace ig

#endif // IG_ENVI_PHYSICAL_H
