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

#include "imagine/nc/imag/image.h"
#include "imagine/nc/imag/io/jpeg.h"
#include "imagine/nc/imag/io/png.h"

#include <algorithm>
#include <numeric>
#include <fstream>

namespace ig {

constexpr image::image()
  : dims_{}, channels_{0}, size_{0}, bit_depth_{0}, pitch_{0} {
}

image::image(access_t dimensions, uint32_t channels, uint32_t bit_depth)
  : dims_{dimensions}, channels_{channels}, size_{1}, bit_depth_{bit_depth} {
  pitch_ = dims_.front() * bit_depth_ / 8;
  for (auto it = dims_.begin() + 1; it != dims_.end(); ++it)
    size_ *= (*it);

  size_ *= pitch_;
  pixels_.resize(size_ * channels_);
}

const uint8_t& image::operator[](access_t coords) const {
  size_t acc = 0; auto it = coords.end();
  for (auto dim = dims_.size(); dim--> 1; )
    acc += (*--it) * std::accumulate(dims_.begin(), dims_.begin() + dim, 1, std::multiplies<>{});

  auto id = ((*--it) + acc) * (pitch_ / dims_.front());
  return pixels_[id];
}

uint8_t& image::operator[](access_t coords) {
  size_t acc = 0; auto it = coords.end();
  for (auto dim = dims_.size(); dim--> 1; )
    acc += (*--it) * std::accumulate(dims_.begin(), dims_.begin() + dim, 1, std::multiplies<>{});

  auto id = ((*--it) + acc) * (pitch_ / dims_.front());
  return pixels_[id];
}

bool image::save(format_t format, const std::string& filename) {
  using namespace detail;

  std::ofstream file{filename, std::ios::binary | std::ios::trunc};
  if (!file.good()) return false;

  switch (format) {
  case format_t::png:
    return png_write(*this, file);
  case format_t::jpeg:
    return jpeg_write(*this, file);
  default:
    return false;
  }
}

std::unique_ptr<image> image::load(const std::string& filename) {
  using namespace detail;

  std::ifstream file{filename, std::ios::binary};
  if (!file.good()) return nullptr;

  if (jpeg_validate(file)) return jpeg_read(file);
  if (png_validate(file))  return png_read(file);
  return nullptr;
}

} // namespace ig
