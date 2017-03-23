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

#ifndef IG_ENVI_VK_IMAGE_H
#define IG_ENVI_VK_IMAGE_H

#include "imagine/envi/_vk/resource.h"

namespace ig {
namespace vk {

enum class image_layout;
enum class image_usage  : uint32_t; using image_usages  = flags<image_usage>;
enum class image_aspect : uint32_t; using image_aspects = flags<image_aspect>;

class ig_api image : public managed<VkImage_T*>, public resource {
public:
  enum type_t { s1d = 0, s2d = 1, s3d = 2 };
  enum view_type_t {
    view_1d       = 0, view_2d = 1, view_3d = 2,
    view_cube     = 3, 
    view_1d_array = 4, view_2d_array = 5, view_cube_array = 6 };
  using iview = VkImageView_T*;
  class sampler;

  explicit image(const device& device, VkImage_T* image, format format);
  explicit image(const device& device, type_t type, 
                   format format, image_usages usages,
                     uint32_t width, uint32_t height, uint32_t depth, uint32_t layers = 1, uint32_t mips = 1);
  virtual ~image();

  auto view(view_type_t type, image_aspects asp, uint32_t blayer = 0, uint32_t layers = 1, uint32_t bmip = 0, uint32_t mips = 1) 
    -> iview&;
  auto& get_format() const { return format_; }

private:
  bool bind(std::unique_ptr<block_type> block) override;
  auto require(uint64_t& size, uint64_t& alignment) -> uint32_t override;

private:
  format format_;
  std::unordered_map<view_type_t, iview> views_;
};

enum class image_layout {
  undefined = 0, general = 1,
  color           = 2,
  depth_stencil   = 3,
  shader_readonly = 5,
  transfer_src = 6, transfer_dst = 7, present = 1000001002 };

enum class image_usage : uint32_t {
  transfer_src = 0x001, transfer_dst  = 0x002,
  sampled      = 0x004, storage       = 0x008,
  color        = 0x010, depth_stencil = 0x020,
  transient    = 0x040, input         = 0x080 };

enum class image_aspect : uint32_t {
  color = 0x001, depth = 0x002, stencil = 0x004, metadata = 0x008 };

} // namespace vk
} // namespace ig

#endif // IG_ENVI_VK_IMAGE_H
