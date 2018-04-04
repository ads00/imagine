/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_SIMULATION_PNG_H
#define IG_SIMULATION_PNG_H

#include "imagine/simulation/world/data/signal/base_signal.h"
#include <sstream>

namespace ig     {
namespace detail {

using png   = image_bridge::data;
using png_t = image_bridge::resource;
// libpng (1.6.28)
// supports lossless grayscale and rgb with alpha channel
// extensions -> .png
// validate -> 89 50 4e 47 0d 0a 1a 0a
// standard -> ISO/IEC 15948

bool png_validate(std::istream& stream);
png  png_readp_impl(std::istream& stream, const image_bridge::parameters&);
bool png_write_impl(std::ostream& stream, const image_bridge::parameters&, const image_bridge::resource& image);

} // namespace detail
} // namespace ig

#endif // IG_SIMULATION_PNG_H
