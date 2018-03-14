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

#include "imagine/simulation/world/data/signal/image/jpeg.h"
#include "imagine/simulation/world/data/signal/image/png.h"

#include "imagine/simulation/world/data/mesh/obj.h"

#include <tuple>

namespace ig {

template <>
IG_API image_bridge::table& image_bridge::get() {
  static image_bridge::table tbl =
  {{
  {std::make_tuple(detail::jpeg_validate, detail::jpeg_readp_impl, detail::jpeg_write_impl)}, // jpeg
  {std::make_tuple(detail:: png_validate, detail:: png_readp_impl, detail:: png_write_impl)}, // png
  {std::make_tuple(nullptr, nullptr, nullptr)},                                               // bmp
  {std::make_tuple(nullptr, nullptr, nullptr)},                                               // pam
  {std::make_tuple(nullptr, nullptr, nullptr)}                                                // hdr
  }}; return tbl;
}

template <>
IG_API audio_bridge::table& audio_bridge::get() {
  static audio_bridge::table tbl =
  {{
  {std::make_tuple(nullptr, nullptr, nullptr)},                                               // flac
  {std::make_tuple(nullptr, nullptr, nullptr)},                                               // mp3
  {std::make_tuple(nullptr, nullptr, nullptr)},                                               // ogg
  {std::make_tuple(nullptr, nullptr, nullptr)}                                                // wav
  }}; return tbl;
}

template <>
IG_API mesh_bridge::table& mesh_bridge::get() {
  static mesh_bridge::table tbl =
  {{
  {std::make_tuple(nullptr, nullptr, nullptr)},                                           // fbx
  {std::make_tuple(nullptr, nullptr, nullptr)},                                           // dae
  {std::make_tuple(nullptr, nullptr, nullptr)},                                           // stl
  {std::make_tuple(detail::obj_validate, detail::obj_readp_impl, detail::obj_write_impl)} // obj
  }}; return tbl;
}

} // namespace ig
