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

#include "imagine/simulation/world/data/signal/image/png.h"
#include "imagine/core/log.h"

extern "C"
{
#include "png/png.h"
#include "png/zlib.h"
}

namespace ig     {
namespace detail {

struct png_src { std::istream* stream; };
struct png_dst { std::ostream* stream; };

void png_readproc(png_structp png_ptr,
                  png_bytep data,
                  png_size_t size);
void png_writeproc(png_structp png_ptr,
                   png_bytep data,
                   png_size_t size);

void png_flushproc(png_structp png_ptr);
void png_message(png_structp png_ptr, const char* msg);

// Png interface implementation - validate - read - write
bool png_validate(std::istream& stream) {
  png_byte png_sig[8];

  stream.read(reinterpret_cast<char*>(png_sig), 8);
  stream.seekg(0, std::ios::beg);
  return !png_sig_cmp(png_sig, 0, 8);
}

png png_readp_impl(std::istream& stream, const image_bridge::parameters&) {
  png_src src{&stream};
  png_structp png_ptr =
    png_create_read_struct(
      PNG_LIBPNG_VER_STRING,
      nullptr,
      png_message,
      png_message);
  png_infop info_ptr = png_create_info_struct(png_ptr);

  png_set_read_fn(
    png_ptr,
    &src,
    png_readproc);
  png_read_info(png_ptr, info_ptr);

  png_set_palette_to_rgb(png_ptr);
  png_set_expand_gray_1_2_4_to_8(png_ptr);
  png_set_scale_16(png_ptr);

  png_read_update_info(png_ptr, info_ptr);

  auto channels = png_get_channels(png_ptr, info_ptr);
  auto width    = png_get_image_width (png_ptr, info_ptr),
       height   = png_get_image_height(png_ptr, info_ptr);

  auto image =
  std::make_unique<png_t>(std::initializer_list<uint32_t>{channels, width, height});

  for (png_uint_32 j = 0; j < height; ++j)
    png_read_row(
      png_ptr,
      reinterpret_cast<uint8_t*>(image->buffer() + (width * channels * j)),
      nullptr);

  png_read_end(png_ptr, info_ptr);
  png_destroy_read_struct(
    &png_ptr,
    &info_ptr,
    nullptr);
  return std::make_pair(true, std::move(image));
}

bool png_write_impl(std::ostream& stream, const image_bridge::parameters&, const image_bridge::resource& image) {
  png_dst dst{&stream};
  png_structp png_ptr =
    png_create_write_struct(
      PNG_LIBPNG_VER_STRING,
      nullptr,
      png_message,
      png_message);
  png_infop info_ptr = png_create_info_struct(png_ptr);

  auto channels = image.shape()[0],
       width    = image.shape()[1],
       height   = image.shape()[2];

  int colortype;
  switch (channels) {
  case 1: colortype = PNG_COLOR_TYPE_GRAY; break; case 2: colortype = PNG_COLOR_TYPE_GRAY_ALPHA; break;
  case 3: colortype = PNG_COLOR_TYPE_RGB;  break; case 4: colortype = PNG_COLOR_TYPE_RGB_ALPHA;  break; default: return false; }

  png_set_compression_level(png_ptr, 6);
  if (channels >= 2) {
    png_set_compression_strategy(png_ptr, Z_FILTERED);
    png_set_filter(
      png_ptr,
      0,
      PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_PAETH);
  } else {
    png_set_compression_strategy(png_ptr, Z_DEFAULT_STRATEGY);
  }

  png_set_IHDR(
    png_ptr,
    info_ptr,
    width, height,
    std::numeric_limits<uint8_t>::digits, colortype,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,  PNG_FILTER_TYPE_BASE);

  png_set_write_fn(
    png_ptr,
    &dst,
    png_writeproc,
    png_flushproc);
  png_write_info(png_ptr, info_ptr);

  for (png_uint_32 j = 0; j < height; ++j)
    png_write_row(
      png_ptr,
      reinterpret_cast<const uint8_t*>(image.buffer() + (width * channels * j)));

  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(
    &png_ptr,
    &info_ptr);
  return true;
}

void png_readproc(png_structp png_ptr,
                  png_bytep data,
                  png_size_t size) {
  auto src = reinterpret_cast<png_src*>(png_get_io_ptr(png_ptr));
  src->stream->read(reinterpret_cast<char*>(data), size);
}

void png_writeproc(png_structp png_ptr,
                   png_bytep data,
                   png_size_t size) {
  auto dst = reinterpret_cast<png_dst*>(png_get_io_ptr(png_ptr));
  dst->stream->write(reinterpret_cast<char*>(data), size);
}

void png_flushproc(png_structp png_ptr) {}
void png_message(png_structp png_ptr, const char* msg) {
  log_(info, "[libpng] {}", msg);
}

} // namespace detail
} // namespace ig
