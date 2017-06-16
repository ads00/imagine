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

#ifndef IG_ENVI_VK_DEVICE_H
#define IG_ENVI_VK_DEVICE_H

#include "imagine/envi/impl_vk/detail/wrapper.h"
#include "imagine/envi/impl_vk/detail/physical.h"

namespace ig {
namespace vk {

class dPfn;
class memory_allocator;
class ig_api device : public managed<VkDevice_T*> {
public:
  friend vulkan;
  friend class queue;

  explicit device(const physical& physical, const std::vector<capabilities>& rq);
  virtual ~device();

  bool wait() const;
  bool supported(const std::string& name) const;

  template <size_t S>
  auto get_queues() const { return gen_context_queues(std::make_index_sequence<S>{}); }
  template <size_t... Is>
  auto gen_context_queues(std::index_sequence<Is...>) const {
    return std::make_tuple(
      std::move([this](uint32_t i) { return std::make_unique<queue>(*this, i); }(Is))...);
  }

  auto& get_allocator() const { return allocator_; }
  auto& operator->() const    { return dpfn_; }

  const physical& phys;
  const instance& inst;

private:
  virtual void pre_acquire() override;

private:
  std::unique_ptr<memory_allocator> 
    allocator_;
  struct select {
    std::vector< std::pair<uint32_t, uint32_t> > indices;     // indices[i]     -> family , index
    std::vector< std::pair<uint32_t, uint32_t> > definitions; // definitions[i] -> family , count
    void prepare(uint32_t family);
    auto retrieve(uint32_t i) const { return i < indices.size() ? indices[i] : std::make_pair(0, 0); }
  } selects_;

  struct impl;
  std::unique_ptr<impl> impl_;
  std::unique_ptr<dPfn> dpfn_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_DEVICE_H
