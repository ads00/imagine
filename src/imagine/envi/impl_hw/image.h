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

#ifndef IG_ENVI_IMAGE_H
#define IG_ENVI_IMAGE_H

#include "imagine/envi/impl_hw/memory.h"

#include <unordered_map>

namespace ig {

enum class image_format;
enum class image_layout;
enum class image_usage  
  : uint32_t; using image_usages  = flags<image_usage>;
enum class image_aspect 
  : uint32_t; using image_aspects = flags<image_aspect>;

class IG_API image : public VKObject<VkImage>, public memory::resource {
public:
  enum type_t { s1d = 0, s2d = 1, s3d = 2 };
  enum view_type_t {
    view_1d       = 0, view_2d = 1, view_3d = 2,
    view_cube     = 3, 
    view_1d_array = 4, view_2d_array = 5, view_cube_array = 6 };
  using iview = VkImageView;
  friend memory_allocator;

  explicit image(const device& device, VkImage image, image_format format);
  explicit image(const device& device, type_t type, 
                   image_format format, image_usages usages,
                     uint32_t width, uint32_t height, uint32_t depth, uint32_t layers = 1, uint32_t mips = 1);
  virtual ~image();

  auto view(view_type_t type, image_aspects aspects, 
              uint32_t blayer = 0, uint32_t layers = 1, uint32_t bmip = 0, uint32_t mips = 1) -> iview&;

  auto& get_format() const { return format_; }

  const device& devi;

private:
  void bind(memory& memory, memory::block_iterator block, uint64_t aligned) override;
  auto requirements(uint64_t& size, uint64_t& alignment) -> uint32_t override;

private:
  image_format format_;
  std::unordered_map<view_type_t, iview> views_;
};

enum class image_format {
  undefined = 0,
  r4g4_unorm_pack8 = 1, r4g4b4a4_unorm_pack16 = 2, b4g4r4a4_unorm_pack16 = 3,
  r5g6b5_unorm_pack16 = 4, b5g6r5_unorm_pack16 = 5, r5g5b5a1_unorm_pack16 = 6, b5g5r5a1_unorm_pack16 = 7, a1r5g5b5_unorm_pack16 = 8,

  r8_unorm = 9,  r8_snorm = 10, r8_uscaled = 11, r8_sscaled = 12,
  r8_uint  = 13, r8_sint  = 14, r8_srgb    = 15,
  r8g8_unorm = 16, r8g8_snorm = 17, r8g8_uscaled = 18, r8g8_sscaled = 19,
  r8g8_uint  = 20, r8g8_sint  = 21, r8g8_srgb    = 22,
  r8g8b8_unorm = 23, r8g8b8_snorm = 24, r8g8b8_uscaled = 25, r8g8b8_sscaled = 26,
  r8g8b8_uint  = 27, r8g8b8_sint  = 28, r8g8b8_srgb    = 29,
  b8g8r8_unorm = 30, b8g8r8_snorm = 31, b8g8r8_uscaled = 32, b8g8r8_sscaled = 33,
  b8g8r8_uint  = 34, b8g8r8_sint  = 35, b8g8r8_srgb    = 36,
  r8g8b8a8_unorm = 37, r8g8b8a8_snorm = 38, r8g8b8a8_uscaled = 39, r8g8b8a8_sscaled = 40,
  r8g8b8a8_uint  = 41, r8g8b8a8_sint  = 42, r8g8b8a8_srgb    = 43,
  b8g8r8a8_unorm = 44, b8g8r8a8_snorm = 45, b8g8r8a8_uscaled = 46, b8g8r8a8_sscaled = 47,
  b8g8r8a8_uint  = 48, b8g8r8a8_sint  = 49, b8g8r8a8_srgb    = 50,

  a8b8g8r8_unorm_pack32 = 51, a8b8g8r8_snorm_pack32 = 52, a8b8g8r8_uscaled_pack32 = 53, a8b8g8r8_sscaled_pack32 = 54,
  a8b8g8r8_uint_pack32  = 55, a8b8g8r8_sint_pack32  = 56, a8b8g8r8_srgb_pack32    = 57,
  a2r10g10b10_unorm_pack32 = 58, a2r10g10b10_snorm_pack32 = 59, a2r10g10b10_uscaled_pack32 = 60, a2r10g10b10_sscaled_pack32 = 61,
  a2r10g10b10_uint_pack32  = 62, a2r10g10b10_sint_pack32  = 63, 
  a2b10g10r10_unorm_pack32 = 64, a2b10g10r10_snorm_pack32 = 65, a2b10g10r10_uscaled_pack32 = 66, a2b10g10r10_sscaled_pack32 = 67,
  a2b10g10r10_uint_pack32  = 68, a2b10g10r10_sint_pack32  = 69,

  r16_unorm = 70, r16_snorm = 71, r16_uscaled = 72, r16_sscaled = 73,
  r16_uint  = 74, r16_sint  = 75, r16_sfloat  = 76,
  r16g16_unorm = 77, r16g16_snorm = 78, r16g16_uscaled = 79, r16g16_sscaled = 80,
  r16g16_uint  = 81, r16g16_sint  = 82, r16g16_sfloat  = 83,
  r16g16b16_unorm = 84, r16g16b16_snorm = 85, r16g16b16_uscaled = 86, r16g16b16_sscaled = 87,
  r16g16b16_uint  = 88, r16g16b16_sint  = 89, r16g16b16_sfloat  = 90,
  r16g16b16a16_unorm = 91, r16g16b16a16_snorm = 92, r16g16b16a16_uscaled = 93, r16g16b16a16_sscaled = 94,
  r16g16b16a16_uint  = 95, r16g16b16a16_sint  = 96, r16g16b16a16_sfloat  = 97,

  r32_uint          = 98,  r32_sint          = 99,  r32_sfloat          = 100,
  r32g32_uint       = 101, r32g32_sint       = 102, r32g32_sfloat       = 103,
  r32g32b32_uint    = 104, r32g32b32_sint    = 105, r32g32b32_sfloat    = 106,
  r32g32b32a32_uint = 107, r32g32b32a32_sint = 108, r32g32b32a32_sfloat = 109,
  r64_uint          = 110, r64_sint          = 111, r64_sfloat          = 112,
  r64g64_uint       = 113, r64g64_sint       = 114, r64g64_sfloat       = 115,
  r64g64b64_uint    = 116, r64g64b64_sint    = 117, r64g64b64_sfloat    = 118,
  r64g64b64a64_uint = 119, r64g64b64a64_sint = 120, r64g64b64a64_sfloat = 121,

  b10g11r11_ufloat_pack32 = 122, e5b9g9r9_ufloat_pack32 = 123,

  d16_unorm = 124,
  x8_d24_unorm_pack32 = 125,
  d32_sfloat = 126, s8_uint = 127, d16_unorm_s8_uint = 128, d24_unorm_s8_uint = 129, d32_sfloat_s8_uint = 130,

  bc1_rgb_unorm_block = 131, bc1_rgb_srgb_block = 132, bc1_rgba_unorm_block = 133, bc1_rgba_srgb_block = 134,
  bc2_unorm_block     = 135, bc2_srgb_block     = 136, bc3_unorm_block      = 137, bc3_srgb_block      = 138,
  bc4_unorm_block     = 139, bc4_snorm_block    = 140, bc5_unorm_block      = 141, bc5_snorm_block     = 142,
  bc6h_ufloat_block   = 143, bc6h_sfloat_block  = 144, bc7_unorm_block      = 145, bc7_srgb_block      = 146,

  etc2_r8g8b8_unorm_block   = 147, etc2_r8g8b8_srgb_block   = 148,
  etc2_r8g8b8a1_unorm_block = 149, etc2_r8g8b8a1_srgb_block = 150, etc2_r8g8b8a8_unorm_block = 151, etc2_r8g8b8a8_srgb_block = 152,
  eac_r11_unorm_block       = 153, eac_r11_snorm_block      = 154, eac_r11g11_unorm_block    = 155, eac_r11g11_snorm_block   = 156,

  astc_4x4_unorm_block   = 157, astc_4x4_srgb_block   = 158, astc_5x4_unorm_block   = 159, astc_5x4_srgb_block   = 160,
  astc_5x5_unorm_block   = 161, astc_5x5_srgb_block   = 162, astc_6x5_unorm_block   = 163, astc_6x5_srgb_block   = 164,
  astc_6x6_unorm_block   = 165, astc_6x6_srgb_block   = 166, astc_8x5_unorm_block   = 167, astc_8x5_srgb_block   = 168,
  astc_8x6_unorm_block   = 169, astc_8x6_srgb_block   = 170, astc_8x8_unorm_block   = 171, astc_8x8_srgb_block   = 172,
  astc_10x5_unorm_block  = 173, astc_10x5_srgb_block  = 174, astc_10x6_unorm_block  = 175, astc_10x6_srgb_block  = 176,
  astc_10x8_unorm_block  = 177, astc_10x8_srgb_block  = 178, astc_10x10_unorm_block = 179, astc_10x10_srgb_block = 180,
  astc_12x10_unorm_block = 181, astc_12x10_srgb_block = 182, astc_12x12_unorm_block = 183, astc_12x12_srgb_block = 184, };

enum class image_layout {
  undefined    = 0, general       = 1,
  color        = 2, depth_stencil = 3, depth_stencil_readonly = 4, shader_readonly = 5,
  transfer_src = 6, transfer_dst  = 7, preinitialized         = 8, present = 1000001002 };

enum class image_usage : uint32_t {
  transfer_src = 0x001, transfer_dst  = 0x002,
  sampled      = 0x004, storage       = 0x008,
  color        = 0x010, depth_stencil = 0x020,
  transient    = 0x040, input         = 0x080 };

enum class image_aspect : uint32_t {
  color = 0x001, depth = 0x002, stencil = 0x004, metadata = 0x008 };

} // namespace ig

#endif // IG_ENVI_IMAGE_H
