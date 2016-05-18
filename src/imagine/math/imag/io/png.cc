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

#include "imagine/math/imag/io/png.h"
#include "imagine/core/log.h"

#include "png/png.h"
#include "zlib/zlib.h"

#include <memory>
#include <vector>
#include <csetjmp>

namespace ig     {
namespace detail {

struct png_src { std::istream* stream; };
struct png_dst { std::ostream* stream; };

void readproc(png_structp png_ptr, png_bytep data, png_size_t size);
void writeproc(png_structp png_ptr, png_bytep data, png_size_t size);
void flushproc(png_structp png_ptr);

void message(png_structp png_ptr, const char* msg);

std::unique_ptr<image> png_read(std::istream& stream) {
  auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, message, message);
  if (!png_ptr) 
    return nullptr;

  auto info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    return nullptr;
  }

  auto src = png_src{};
  src.stream = &stream;
  png_set_read_fn(png_ptr, &src, readproc);

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return nullptr;
  }

  png_read_info(png_ptr, info_ptr);

  auto width = png_uint_32{}, height = png_uint_32{};
  auto bitdepth = int{}, colortype = int{};

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitdepth, &colortype, nullptr, nullptr, nullptr);
  auto channels = png_get_channels(png_ptr, info_ptr);

  switch (colortype) {
  case PNG_COLOR_TYPE_PALETTE:
    png_set_palette_to_rgb(png_ptr);
    channels = 3;
    break;
  case PNG_COLOR_TYPE_GRAY:
    switch (bitdepth) {
    case 1:
    case 2:
    case 4:
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      break;
    }

    break;
  case PNG_COLOR_TYPE_GRAY_ALPHA:
    png_set_gray_to_rgb(png_ptr);
    break;
  }

  #if IG_BIG_ENDIAN
  if (bitdepth == 16)
    png_set_swap(png_ptr);
  #endif

  png_read_update_info(png_ptr, info_ptr);

  channels = png_get_channels(png_ptr, info_ptr);
  bitdepth = png_get_bit_depth(png_ptr, info_ptr);

  auto dims = {width, height};
  auto imag = std::make_unique<image>(dims, channels, bitdepth);

  auto rowptrs = std::vector<png_bytep>(height * sizeof(png_bytepp));
  for (png_uint_32 j = 0; j < height; ++j) {
    rowptrs[height - 1 - j] = imag->pixels() + (imag->pitch() * imag->channels() * j);
  }

  png_set_benign_errors(png_ptr, 1);
  png_read_image(png_ptr, rowptrs.data());
  png_read_end(png_ptr, info_ptr);

  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
  return imag;
}

bool png_write(const image& imag, std::ostream& stream) {
  auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, message, message);
  if (!png_ptr) return false;

  auto info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;
  }

  auto dst = png_dst{};
  dst.stream = &stream;
  png_set_write_fn(png_ptr, &dst, writeproc, flushproc);

  int colortype;
  switch (imag.channels()) {
  case 1:
    colortype = PNG_COLOR_TYPE_GRAY;
    break;
  case 2:
    colortype = PNG_COLOR_TYPE_GRAY_ALPHA;
    break;
  case 3:
    colortype = PNG_COLOR_TYPE_RGB;
    break;
  case 4:
    colortype = PNG_COLOR_TYPE_RGBA;
    break;
  default:
    return false;
  }

  png_set_compression_level(png_ptr, 6);
  if (imag.channels() * imag.bit_depth() >= 16) {
    png_set_compression_strategy(png_ptr, Z_FILTERED);
    png_set_filter(png_ptr, 0, PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_PAETH);
  } else {
    png_set_compression_strategy(png_ptr, Z_DEFAULT_STRATEGY);
  }

  png_set_IHDR(png_ptr, info_ptr, imag.dimensions()[0], imag.dimensions()[1],
               imag.bit_depth(), colortype, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  #if IG_BIG_ENDIAN
  if (imag.bit_depth() == 16) png_set_swap(png_ptr);
  #endif

  for (png_uint_32 j = 0; j < imag.dimensions()[1]; ++j) {
    png_write_row(png_ptr, imag.pixels() + (imag.pitch() * imag.channels() * (imag.dimensions()[1] - j - 1)));
  }

  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  return true;
}

bool png_validate(std::istream& stream) {
  png_byte png_sig[8];

  stream.read(reinterpret_cast<char*>(png_sig), 8);
  stream.seekg(0, std::ios::beg);
  return !png_sig_cmp(png_sig, 0, 8);
}

void readproc(png_structp png_ptr, png_bytep data, png_size_t size) {
  auto src = reinterpret_cast<png_src*>(png_get_io_ptr(png_ptr));
  src->stream->read(reinterpret_cast<char*>(data), size);
  assert(src->stream->gcount() != 0 && "Invalid or corrupted png file");
}

void writeproc(png_structp png_ptr, png_bytep data, png_size_t size) {
  auto dst = reinterpret_cast<png_dst*>(png_get_io_ptr(png_ptr));
  dst->stream->write(reinterpret_cast<char*>(data), size);
}

void flushproc(png_structp png_ptr) {
}

void message(png_structp png_ptr, const char* msg) {
  LOG(info) << "(libpng): " << msg;
}

} // namespace detail
} // namespace ig
