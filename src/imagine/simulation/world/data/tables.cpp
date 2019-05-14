/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/simulation/world/data/signal/image/jpeg.h"
#include "imagine/simulation/world/data/signal/image/png.h"

#include "imagine/simulation/world/data/mesh/obj.h"

#include <tuple>

namespace ig {

template <>
IG_API image_bridge::table& image_bridge::_() {
  static image_bridge::table tbl =
  {{
  {std::make_tuple(detail::jpeg_v_impl, detail::jpeg_i_impl, detail::jpeg_o_impl)}, // jpeg
  {std::make_tuple(detail:: png_v_impl, detail:: png_i_impl, detail:: png_o_impl)}, // png
  {std::make_tuple(nullptr, nullptr, nullptr)},                                     // bmp
  {std::make_tuple(nullptr, nullptr, nullptr)},                                     // pam
  {std::make_tuple(nullptr, nullptr, nullptr)}                                      // hdr
  }}; return tbl;
}

template <>
IG_API audio_bridge::table& audio_bridge::_() {
  static audio_bridge::table tbl =
  {{
  {std::make_tuple(nullptr, nullptr, nullptr)}, // flac
  {std::make_tuple(nullptr, nullptr, nullptr)}, // mp3
  {std::make_tuple(nullptr, nullptr, nullptr)}, // ogg
  {std::make_tuple(nullptr, nullptr, nullptr)}  // wav
  }}; return tbl;
}

template <>
IG_API mesh_bridge::table& mesh_bridge::_() {
  static mesh_bridge::table tbl =
  {{
  {std::make_tuple(nullptr, nullptr, nullptr)},                                 // fbx
  {std::make_tuple(nullptr, nullptr, nullptr)},                                 // dae
  {std::make_tuple(nullptr, nullptr, nullptr)},                                 // stl
  {std::make_tuple(detail::obj_v_impl, detail::obj_i_impl, detail::obj_o_impl)} // obj
  }}; return tbl;
}

} // namespace ig
