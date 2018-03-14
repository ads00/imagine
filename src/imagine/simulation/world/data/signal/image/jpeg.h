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

#ifndef IG_SIMULATION_JPEG_H
#define IG_SIMULATION_JPEG_H

#include "imagine/simulation/world/data/signal/base_signal.h"
#include <sstream>

namespace ig     {
namespace detail {

using jpeg   = image_bridge::data;
using jpeg_t = image_bridge::resource;
// libjpeg (9b)
// supports lossy grayscale and rgb
// extensions -> .jpg, .jpeg, .jpe, .jif, .jfif, .jfi
// validate -> ff d8 ff
// standard -> ISO/IEC 10918

bool jpeg_validate(std::istream& stream);
jpeg jpeg_readp_impl(std::istream& stream, const image_bridge::parameters&);
bool jpeg_write_impl(std::ostream& stream, const image_bridge::parameters&, const image_bridge::resource& image);

} // namespace detail
} // namespace ig

#endif // IG_SIMULATION_JPEG_H
