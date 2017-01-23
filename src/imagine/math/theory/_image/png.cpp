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

#include "imagine/math/theory/_image/png.h"
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

// Png interface implementation  - validate - read - write
bool png_validate(std::istream& stream) {
  png_byte png_sig[8];

  stream.read(reinterpret_cast<char*>(png_sig), 8); stream.seekg(0, std::ios::beg);
  return !png_sig_cmp(png_sig, 0, 8);
}

template <typename T>
auto png_read(std::istream& stream) -> std::unique_ptr< png_t<T> > {
  struct read_handle { 
    explicit read_handle(png_src& src) {
      png  = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, png_message, png_message);
      info = png_create_info_struct(png);
      if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        throw std::runtime_error{"Failed to create png info structure"};
      }

      png_set_read_fn(png, &src, png_readproc);
      png_read_info(png, info);
    }
    ~read_handle()
    { png_destroy_read_struct(&png, &info, nullptr); }

    auto adaptive_generation() {
      png_get_IHDR(png, info, &width, &height, &bitdepth, &colortype, nullptr, nullptr, nullptr);
      switch (colortype) {
      case PNG_COLOR_TYPE_PALETTE:    png_set_palette_to_rgb(png); break;
      case PNG_COLOR_TYPE_GRAY_ALPHA: png_set_gray_to_rgb(png);    break;
      case PNG_COLOR_TYPE_GRAY:
        switch (bitdepth) {
        case 1:
        case 2:
        case 4: png_set_expand_gray_1_2_4_to_8(png);
        }
      break; }
      switch (sizeof(T) * std::numeric_limits<uint8_t>::digits) {
      case 8:  png_set_scale_16(png);  break;
      case 16: png_set_expand_16(png); break; }

      png_read_update_info(png, info);
      return std::make_unique< png_t<T> >(
        png_t<T>::shape_type{width, height}, png_get_channels(png, info));
    }

    png_structp png; png_infop info; 
    png_uint_32 width, height; int bitdepth, colortype;
  };

  png_src src{&stream};
  auto handler = std::make_unique<read_handle>(src);

  auto imag = handler->adaptive_generation();
  auto rowptrs = std::vector<png_bytep>(handler->height * sizeof(png_bytepp));
  for (png_uint_32 j = 0; j < handler->height; ++j)
    rowptrs[j] = reinterpret_cast<uint8_t*>(
      imag->buffer() + (imag->get_shape().front() * imag->get_features() * j));

  png_set_benign_errors(handler->png, 1);
  png_read_image(handler->png, rowptrs.data()); png_read_end(handler->png, handler->info);
  return imag;
}

template <typename T>
bool png_write(std::ostream& stream, const png_t<T>& imag) {
  struct write_handle {
    explicit write_handle(png_dst& dst) {
      png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, png_message, png_message);
      info = png_create_info_struct(png);
      if (!info) {
        png_destroy_write_struct(&png, nullptr);
        throw std::runtime_error{"Failed to create png info structure"};
      }

      png_set_write_fn(png, &dst, png_writeproc, png_flushproc);
    }
    ~write_handle() 
    { png_destroy_write_struct(&png, &info); }

    bool adaptive_generation(const png_t<T>& imag) {
      int colortype, bitdepth = sizeof(T) * std::numeric_limits<uint8_t>::digits;
      switch (imag.get_features()) {
      case 1: colortype = PNG_COLOR_TYPE_GRAY;       break;
      case 2: colortype = PNG_COLOR_TYPE_GRAY_ALPHA; break;
      case 3: colortype = PNG_COLOR_TYPE_RGB;        break;
      case 4: colortype = PNG_COLOR_TYPE_RGBA;       break;
      default: return false;
      }

      png_set_compression_level(png, 6);
      if (imag.get_features() * bitdepth >= 16) {
        png_set_compression_strategy(png, Z_FILTERED);
        png_set_filter(png, 0, PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_PAETH);
      } else {
        png_set_compression_strategy(png, Z_DEFAULT_STRATEGY);
      }

      png_set_IHDR(png, info, imag.get_shape()[0], imag.get_shape()[1],
                   bitdepth, colortype, PNG_INTERLACE_NONE,
                   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
      png_write_info(png, info);
      return true;
    }

    png_structp png; png_infop info;
  };

  png_dst dst{&stream};
  auto handler = std::make_unique<write_handle>(dst);

  if (!handler->adaptive_generation(imag))
    return false;
  for (png_uint_32 j = 0; j < imag.get_shape()[1]; ++j)
    png_write_row(handler->png, reinterpret_cast<const uint8_t*>(
      imag.buffer() + (imag.get_shape().front() * imag.get_features() * j)));

  png_write_end(handler->png, handler->info);
  png_destroy_write_struct(&handler->png, &handler->info);
  return true;
}

auto png_readp_uint8_t (std::istream& stream) -> std::unique_ptr<png_8>  { return png_read<uint8_t>(stream); }
auto png_readp_uint16_t(std::istream& stream) -> std::unique_ptr<png_16> { return png_read<uint16_t>(stream); }

bool png_write_uint8_t (std::ostream& stream, const png_8&  imag) { return png_write<uint8_t>(stream, imag); }
bool png_write_uint16_t(std::ostream& stream, const png_16& imag) { return png_write<uint16_t>(stream, imag); }

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
  LOG(info) << "(libpng): " << msg;
}

} // namespace detail
} // namespace ig
