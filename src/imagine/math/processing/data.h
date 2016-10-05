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

#ifndef IG_MATH_DATA_H
#define IG_MATH_DATA_H

#include "imagine/ig.h"

#include <vector>
#include <memory>

namespace ig {

class IG_API data {
public:
  enum class io_t { unknown, jpeg, png };
  using dimensions_type = std::initializer_list<uint32_t>;

  data() = default;
  explicit data(dimensions_type dimensions, uint32_t channels);

  auto ptr() const { return ptr_.data(); }
  auto ptr()       { return ptr_.data(); }

  auto& dimensions() const { return dims_; }

  auto& channels() const   { return channels_; }
  auto& pitch() const      { return pitch_; }
  
  const uint8_t& operator[](dimensions_type coords) const;
  uint8_t& operator[](dimensions_type coords);

  bool save(io_t format, const std::string& filename);
  static std::unique_ptr<data> load(const std::string& filename);

private:
  std::vector<uint8_t> ptr_;
  std::vector<uint32_t> dims_;
  uint32_t channels_, size_, pitch_;
};

} // namespace ig

#endif // IG_MATH_DATA_H
