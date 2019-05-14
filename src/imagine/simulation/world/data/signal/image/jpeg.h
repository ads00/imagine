/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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

bool jpeg_v_impl(std::istream& stream);
jpeg jpeg_i_impl(std::istream& stream, const image_bridge::parameters&);
bool jpeg_o_impl(std::ostream& stream, const image_bridge::parameters&, const image_bridge::resource& image);

} // namespace detail
} // namespace ig

#endif // IG_SIMULATION_JPEG_H
