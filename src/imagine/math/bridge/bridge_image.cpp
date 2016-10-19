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

#include "imagine/math/bridge/bridge_image.h"
#include "imagine/math/bridge/impl_image/bridge_jpeg.h"
#include "imagine/math/bridge/impl_image/bridge_png.h"

#include <fstream>

namespace ig {

auto bridge_image::load(const std::string& filename) -> std::unique_ptr<data> {
  std::ifstream in{filename, std::ios::binary};
  if (!in.good()) return nullptr;

  if (impl::jpeg_validate(in)) return impl::jpeg_read(in);
  if (impl::png_validate(in))  return impl::png_read(in);
  return nullptr;
}

bool bridge_image::save(type_t format, const data& image, const std::string& filename) {
  std::ofstream out{filename, std::ios::binary | std::ios::trunc};
  if (!out.good()) return false;

  switch (format) {
  case type_t::jpeg: return impl::jpeg_write(image, out);
  case type_t::png:  return impl::png_write(image, out);
  default: return false;
  }
}

} // namespace ig
