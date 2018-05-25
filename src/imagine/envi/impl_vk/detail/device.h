/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_ENVI_VK_DEVICE_H
#define IG_ENVI_VK_DEVICE_H

#include "imagine/envi/impl_vk/detail/wrapper.h"
#include "imagine/envi/impl_vk/detail/physical.h"

namespace ig {
namespace vk {

class dPfn;
class IG_API device : public managed<VkDevice_T*> {
public:
  friend vulkan;
  friend class queue;

  explicit device(const physical& physical, const std::vector<capabilities>& rq);
  virtual ~device();

  bool wait() const;
  bool is_supported(const std::string& name) const;

  auto& operator->() const    { return dpfn_; }

  const physical& phys;
  const instance& inst;

private:
  void impl_get();

private:
  struct select {
    std::vector< std::pair<uint32_t, uint32_t> > indices;     // indices[i]     -> family , index
    std::vector< std::pair<uint32_t, uint32_t> > definitions; // definitions[i] -> family , count
    void prepare(uint32_t family);
    auto retrieve(uint32_t i) const
    { return i < indices.size() ? indices[i] : std::make_pair(uint32_t(0), uint32_t(0)); }
  } selects_;

  struct impl;
  std::unique_ptr<impl> impl_;
  std::unique_ptr<dPfn> dpfn_;
};

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_DEVICE_H
