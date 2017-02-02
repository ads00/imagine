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

#include "imagine/math/theory/_image/jpeg.h"
#include "imagine/core/log.h"
#include <cstring>

extern "C" 
{
#include "jpeg/jpeglib.h"
#include "jpeg/jerror.h" 
}

namespace ig     {
namespace detail {

constexpr uint32_t buffer_size = 4096;
struct jpeg_src {
  jpeg_source_mgr jpeg;
  std::istream* stream; JOCTET* buffer; };

struct jpeg_dst {
  jpeg_destination_mgr jpeg;
  std::ostream* stream; JOCTET* buffer; };

boolean jpeg_readproc(
  j_decompress_ptr jpeg_ptr);
boolean jpeg_writeproc(
  j_compress_ptr jpeg_ptr);
void jpeg_message(j_common_ptr jpeg_ptr); void jpeg_exit(j_common_ptr jpeg_ptr);

// Jpeg interface implementation - validate - read - write
bool jpeg_validate(std::istream& stream) {
  uint8_t jpeg_sig[] = {0xFF, 0xD8};
  uint8_t sig[2]     = {0, 0};

  stream.read(reinterpret_cast<char*>(sig), sizeof(jpeg_sig)); stream.seekg(0, std::ios::beg);
  return memcmp(jpeg_sig, sig, sizeof(jpeg_sig)) == 0;
}

jptr jpeg_readp_impl(std::istream& stream) {
  jpeg_decompress_struct 
    jpeg_ptr{};
  jpeg_error_mgr 
    jerr{};

  jpeg_ptr.err = jpeg_std_error(&jerr);
  jerr.error_exit = jpeg_exit;
  jerr.output_message = jpeg_message;

  jpeg_create_decompress(&jpeg_ptr);
  
  if (!jpeg_ptr.src) {
    jpeg_ptr.src = reinterpret_cast<jpeg_source_mgr*>((*jpeg_ptr.mem->alloc_small)
                  (reinterpret_cast<j_common_ptr>(&jpeg_ptr), JPOOL_PERMANENT, sizeof(jpeg_src)));

    auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr.src);
    src->buffer = reinterpret_cast<JOCTET*>((*jpeg_ptr.mem->alloc_small)
                 (reinterpret_cast<j_common_ptr>(&jpeg_ptr), JPOOL_PERMANENT, buffer_size * sizeof(JOCTET)));
  }

  auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr.src);
  src->stream = &stream;
  src->jpeg.fill_input_buffer = jpeg_readproc;
  src->jpeg.skip_input_data = [](j_decompress_ptr jpeg_ptr, long bytes) {
    auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr->src);
    if (bytes > 0) {
      while (bytes > static_cast<long>(src->jpeg.bytes_in_buffer)) {
        bytes -= static_cast<long>(src->jpeg.bytes_in_buffer);
        jpeg_readproc(jpeg_ptr);
      }
      src->jpeg.next_input_byte += bytes;
      src->jpeg.bytes_in_buffer -= bytes;
    }
  };

  src->jpeg.init_source = [](j_decompress_ptr) {};
  src->jpeg.term_source = [](j_decompress_ptr) {};

  src->jpeg.resync_to_restart = jpeg_resync_to_restart;
  src->jpeg.bytes_in_buffer = 0;
  src->jpeg.next_input_byte = nullptr;

  jpeg_read_header(&jpeg_ptr, true);
  jpeg_start_decompress(&jpeg_ptr);

  auto imag = std::make_unique<jpeg_type>(
    jpeg_type::shape_type{jpeg_ptr.output_width, jpeg_ptr.output_height}, jpeg_ptr.output_components);

  while (jpeg_ptr.output_scanline < jpeg_ptr.output_height) {
    auto r = imag->buffer() + (imag->get_shape().front() * imag->get_features() * jpeg_ptr.output_scanline);
    jpeg_read_scanlines(&jpeg_ptr, (JSAMPARRAY)&r, 1);
  }

  jpeg_finish_decompress(&jpeg_ptr);
  jpeg_destroy_decompress(&jpeg_ptr);
  return imag;
}

bool jpeg_write_impl(std::ostream& stream, const jpeg_type& imag) {
  jpeg_compress_struct 
    jpeg_ptr{};
  jpeg_error_mgr 
    jerr{};

  jpeg_ptr.err = jpeg_std_error(&jerr);
  jerr.error_exit     = jpeg_exit;
  jerr.output_message = jpeg_message;

  jpeg_create_compress(&jpeg_ptr);

  if (!jpeg_ptr.dest) {
    jpeg_ptr.dest = reinterpret_cast<jpeg_destination_mgr*>((*jpeg_ptr.mem->alloc_small)
                   (reinterpret_cast<j_common_ptr>(&jpeg_ptr), JPOOL_PERMANENT, sizeof(jpeg_dst)));
  }

  auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr.dest);
  dst->stream = &stream;
  dst->jpeg.empty_output_buffer = &jpeg_writeproc;
  dst->jpeg.init_destination = [](j_compress_ptr compress) {
    auto dst = reinterpret_cast<jpeg_dst*>(compress->dest);
    dst->buffer = reinterpret_cast<JOCTET*>((*compress->mem->alloc_small)
                 (reinterpret_cast<j_common_ptr>(compress), JPOOL_IMAGE, buffer_size * sizeof(JOCTET)));

    dst->jpeg.next_output_byte = dst->buffer;
    dst->jpeg.free_in_buffer = buffer_size;
  };

  dst->jpeg.term_destination = [](j_compress_ptr jpeg_ptr) {
    auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr->dest);
    auto count = buffer_size - dst->jpeg.free_in_buffer;

    if (count > 0) {
      dst->stream->write(reinterpret_cast<char*>(dst->buffer), count);
    }
  };

  jpeg_ptr.image_width = imag.get_shape()[0], jpeg_ptr.image_height = imag.get_shape()[1];

  switch (imag.get_features()) {
  case 1:
    jpeg_ptr.in_color_space = JCS_GRAYSCALE;
    jpeg_ptr.input_components = 1;
    break;
  case 3:
    jpeg_ptr.in_color_space = JCS_RGB;
    jpeg_ptr.input_components = 3;
    break;
  default:
    return false;
  }

  jpeg_set_defaults(&jpeg_ptr);
  jpeg_set_quality(&jpeg_ptr, 95, true);

  jpeg_start_compress(&jpeg_ptr, true);

  while (jpeg_ptr.next_scanline < jpeg_ptr.image_height) {
    auto r = imag.buffer() + (imag.get_shape().front() * imag.get_features() * jpeg_ptr.next_scanline);
    jpeg_write_scanlines(&jpeg_ptr, (JSAMPARRAY)&r, 1);
  }

  jpeg_finish_compress(&jpeg_ptr);
  jpeg_destroy_compress(&jpeg_ptr);
  return true;
}

boolean jpeg_readproc(
  j_decompress_ptr jpeg_ptr) {
  auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr->src);
  src->stream->read(reinterpret_cast<char*>(src->buffer), buffer_size);

  src->jpeg.next_input_byte = src->buffer;
  src->jpeg.bytes_in_buffer = (size_t)src->stream->gcount();
  return true;
}

boolean jpeg_writeproc(
  j_compress_ptr jpeg_ptr) {
  auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr->dest);
  dst->stream->write(reinterpret_cast<char*>(dst->buffer), buffer_size);
  
  dst->jpeg.next_output_byte = dst->buffer;
  dst->jpeg.free_in_buffer = buffer_size;
  return true;
}

void jpeg_message(j_common_ptr jpeg_ptr) {
  char buffer[JMSG_LENGTH_MAX];
  jpeg_ptr->err->format_message(jpeg_ptr, buffer); LOG(info) << "(libjpeg): " << buffer;
}

void jpeg_exit(j_common_ptr jpeg_ptr) {
  jpeg_ptr->err->output_message(jpeg_ptr);
  if (jpeg_ptr->err->msg_code != JERR_UNKNOWN_MARKER) {
    jpeg_destroy(jpeg_ptr);
  }
}

} // namespace detail
} // namespace ig
