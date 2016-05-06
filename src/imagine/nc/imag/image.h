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

#ifndef IG_NC_IMAGE_H
#define IG_NC_IMAGE_H

#include "imagine/ig.h"

#include <vector>
#include <memory>

namespace ig {

class IG_API image {
public:
  enum format_t { unknown, jpeg, png };
  using access_t = std::initializer_list<unsigned int>;

  constexpr image();
  image(access_t dimensions, unsigned int channels, unsigned int bit_depth);

  auto pixels() const { return pixels_.data(); }
  auto pixels()             { return pixels_.data(); }

  auto& dimensions() const { return dims_; }

  auto width() const  { return dims_[0]; }
  auto height() const { return dims_[1]; }
  auto depth() const  { return dims_[2]; }

  auto channels() const  { return channels_; }
  auto bit_depth() const { return bit_depth_; }
  auto pitch() const     { return pitch_; }
  
  const uint8_t& operator[](access_t coords) const;
  uint8_t& operator[](access_t coords);

  bool save(format_t format, const std::string& filename);
  static std::unique_ptr<image> load(const std::string& filename);

private:
  std::vector<uint8_t> pixels_;
  std::vector<unsigned int> dims_;
  unsigned int channels_, size_, bit_depth_, pitch_;
};

} // namespace ig

#endif // IG_NC_IMAGE_H
