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

#include "imagine/graphics/import/jpeg.h"
#include "imagine/core/log.h"
#include <memory>

#include "jpeg/jpeglib.h"
#include "jpeg/jerror.h"
#include <csetjmp>

namespace ig     {
namespace detail {

struct jpeg_src
{
  jpeg_source_mgr jpeg;
  std::istream* stream;

  JOCTET* buffer;
};

struct jpeg_dst
{
  jpeg_destination_mgr jpeg;
  std::ostream* stream;

  JOCTET* buffer;
};

struct jpeg_error
{
  jpeg_error_mgr jpeg;
  jmp_buf setjmp_buffer;
};

boolean readproc(j_decompress_ptr jpeg_ptr);
boolean writeproc(j_compress_ptr jpeg_ptr);

void message(j_common_ptr jpeg_ptr);
void exit(j_common_ptr jpeg_ptr);

constexpr unsigned int buffer_in  = 4096;
constexpr unsigned int buffer_out = 4096;

std::unique_ptr<image> jpeg_read(std::istream& stream)
{
  jpeg_decompress_struct jpeg_ptr;
  jpeg_error err;

  jpeg_ptr.err = jpeg_std_error(&err.jpeg);
  err.jpeg.output_message = message;
  err.jpeg.error_exit = exit;

  if (setjmp(err.setjmp_buffer))
  {
    jpeg_destroy_decompress(&jpeg_ptr);
    return nullptr;
  }

  jpeg_create_decompress(&jpeg_ptr);
  
  if (!jpeg_ptr.src)
  {
    jpeg_ptr.src = (jpeg_source_mgr*)(*jpeg_ptr.mem->alloc_small)
                ((j_common_ptr)&jpeg_ptr, JPOOL_PERMANENT, sizeof(jpeg_src));

    auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr.src);
    src->buffer = (JOCTET*)(*jpeg_ptr.mem->alloc_small)
                  ((j_common_ptr)&jpeg_ptr, JPOOL_PERMANENT, buffer_in * sizeof(JOCTET));
  }

  auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr.src);
  src->stream = &stream;
  src->jpeg.fill_input_buffer = readproc;
  src->jpeg.skip_input_data = [](j_decompress_ptr jpeg_ptr, long bytes)
  {
    auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr->src);
    if (bytes > 0)
    {
      while (bytes > (long)src->jpeg.bytes_in_buffer)
      {
        bytes -= (long)src->jpeg.bytes_in_buffer;
        readproc(jpeg_ptr);
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

  const auto dims = {jpeg_ptr.output_width, jpeg_ptr.output_height};
  auto imag = std::make_unique<image>(dims, jpeg_ptr.output_components, 8);

  while (jpeg_ptr.output_scanline < jpeg_ptr.output_height)
  {
    JSAMPROW r = imag->pixels() + (imag->pitch() * imag->channels() * (jpeg_ptr.output_height - jpeg_ptr.output_scanline - 1));
    jpeg_read_scanlines(&jpeg_ptr, &r, 1);
  }

  jpeg_finish_decompress(&jpeg_ptr);
  jpeg_destroy_decompress(&jpeg_ptr);

  return imag;
}

bool jpeg_write(const image& imag, std::ostream& stream)
{
  jpeg_compress_struct jpeg_ptr;
  jpeg_error err;

  jpeg_ptr.err = jpeg_std_error(&err.jpeg);
  err.jpeg.output_message = message;
  err.jpeg.error_exit = exit;
  
  if (setjmp(err.setjmp_buffer))
  {
    jpeg_destroy_compress(&jpeg_ptr);
    return false;
  }

  jpeg_create_compress(&jpeg_ptr);

  if (!jpeg_ptr.dest)
  {
    jpeg_ptr.dest = (jpeg_destination_mgr*)(*jpeg_ptr.mem->alloc_small)
                    ((j_common_ptr)&jpeg_ptr, JPOOL_PERMANENT, sizeof(jpeg_destination_mgr));
  }

  auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr.dest);
  dst->stream = &stream;
  dst->jpeg.empty_output_buffer = writeproc;

  dst->jpeg.init_destination = [](j_compress_ptr jpeg_ptr)
  {
    auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr->dest);
    dst->buffer = (JOCTET*)(*jpeg_ptr->mem->alloc_small)
                  ((j_common_ptr)jpeg_ptr, JPOOL_IMAGE, buffer_out * sizeof(JOCTET));

    dst->jpeg.next_output_byte = dst->buffer;
    dst->jpeg.free_in_buffer = buffer_out;
  };

  dst->jpeg.term_destination = [](j_compress_ptr jpeg_ptr)
  {
    auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr->dest);
    const auto count = buffer_out - dst->jpeg.free_in_buffer;

    if (count > 0)
      dst->stream->write(reinterpret_cast<char*>(dst->buffer), count);
  };

  jpeg_ptr.image_width = imag.width();
  jpeg_ptr.image_height = imag.height();

  switch (imag.channels())
  {
  case 1:
    jpeg_ptr.in_color_space = JCS_GRAYSCALE;
    jpeg_ptr.input_components = 1;
    break;

  case 3:
    jpeg_ptr.in_color_space = JCS_RGB;
    jpeg_ptr.input_components = 3;
    break;

  default:
    IG_LOG(log_t::warn) << "Unsupported jpeg image format";
    return false;
  }

  jpeg_set_defaults(&jpeg_ptr);
  jpeg_set_quality(&jpeg_ptr, 95, true);

  jpeg_start_compress(&jpeg_ptr, true);

  while (jpeg_ptr.next_scanline < jpeg_ptr.image_height)
  {
    JSAMPROW r = (JSAMPROW)imag.pixels() + (imag.pitch() * imag.channels() * (jpeg_ptr.image_height - jpeg_ptr.next_scanline - 1));
    jpeg_write_scanlines(&jpeg_ptr, &r, 1);
  }

  jpeg_finish_compress(&jpeg_ptr);
  jpeg_destroy_compress(&jpeg_ptr);

  return true;
}

bool jpeg_validate(std::istream& stream)
{
  uint8_t jpeg_sig[] = {0xFF, 0xD8};
  uint8_t sig[2] = {0, 0};

  stream.read(reinterpret_cast<char*>(sig), sizeof(jpeg_sig));
  stream.clear();
  stream.seekg(0, std::ios::beg);

  return (memcmp(jpeg_sig, sig, sizeof(jpeg_sig)) == 0);
}

boolean readproc(j_decompress_ptr jpeg_ptr)
{
  auto src = reinterpret_cast<jpeg_src*>(jpeg_ptr->src);
  src->stream->read(reinterpret_cast<char*>(src->buffer), buffer_in);
  assert(src->stream->gcount() != 0 && "Invalid or corrupted jpeg file");

  src->jpeg.next_input_byte = src->buffer;
  src->jpeg.bytes_in_buffer = (std::size_t)src->stream->gcount();

  return true;
}

boolean writeproc(j_compress_ptr jpeg_ptr)
{
  auto dst = reinterpret_cast<jpeg_dst*>(jpeg_ptr->dest);
  dst->stream->write(reinterpret_cast<char*>(dst->buffer), buffer_out);
  
  dst->jpeg.next_output_byte = dst->buffer;
  dst->jpeg.free_in_buffer = buffer_out;

  return true;
}

void message(j_common_ptr jpeg_ptr)
{
  char buffer[JMSG_LENGTH_MAX];
  auto err = reinterpret_cast<jpeg_error*>(jpeg_ptr->err);

  err->jpeg.format_message(jpeg_ptr, buffer);
  IG_LOG(log_t::warn) << "libjpeg message: " << buffer;
}

void exit(j_common_ptr jpeg_ptr)
{
  auto err = reinterpret_cast<jpeg_error*>(jpeg_ptr->err);
  err->jpeg.output_message(jpeg_ptr);

  if (err->jpeg.msg_code != JERR_UNKNOWN_MARKER)
  {
    jpeg_destroy(jpeg_ptr);
    longjmp(err->setjmp_buffer, 1);
  }
}

} // namespace detail
} // namespace ig
