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
