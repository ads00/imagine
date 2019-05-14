/*
 Imagine v0.1
 [bridge]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_SIMULATION_BRIDGE_H
#define IG_SIMULATION_BRIDGE_H

#include "imagine/ig.h"
#include "imagine/core/settings/serialize.h"

#include "imagine/math/theory/graph.h"
#include "imagine/math/theory/matrix.h"
#include "imagine/math/theory/ndarray.h"

#include "imagine/math/geom/representation/mesh.h"

namespace ig {

template
< typename T,
  typename Format,
  typename Params = std::nullptr_t,
  typename Container = std::unique_ptr<T> >
class bridge {
public:
  using resource   = T;
  using parameters = Params;
  using exchange   = Container;
  using data       = std::tuple<bool, exchange>;

  using validate = bool(*)(std::istream&);
  using in       = data(*)(std::istream&, const parameters&);
  using out      = bool(*)(std::ostream&, const parameters&, const resource&);
  using loader =
    std::tuple
      < validate,
        in,
        out >;

  template <Format fmt>
  static bool transform(std::ostream& os, const std::string& name, const parameters& params, const resource& data);
  static auto transform(std::istream& is, const std::string& name, const parameters& params);

private:
  using table = std::vector<loader>;
  static IG_API table& _();
};

template
< typename T,
  typename Format,
  typename Params,
  typename Container >
template <Format fmt>
bool bridge<T, Format, Params, Container>::transform(std::ostream& os, const std::string& name, const parameters& params, const resource& data) {
  if (!os.good()) {
    throw std::runtime_error{
      "[Bridge]: Invalid output stream "
      "(inexistant, unavailable or inaccessible virtual resource)"};
  }

  auto& write = std::get<2>(_()[static_cast<size_t>(fmt)]);
  return os.good()
    ? write(os, params, data)
    : false;
}

template
< typename T,
  typename Format,
  typename Params,
  typename Container >
auto bridge<T, Format, Params, Container>::transform(std::istream& is, const std::string& name, const parameters& params) {
  assert(!name.empty() && "Invalid resource name, failing search management and loader validation");
  if (!is.good()) {
    throw std::runtime_error{
      "[Bridge] Invalid input stream "
      "(inexistant, unavailable or inaccessible virtual resource)"};
  }

  for (auto& entry : _()) {
    auto& validate = std::get<0>(entry);
    if (!validate) continue;

    if (validate(is)) {
      auto&& [s, ptr] = std::get<1>(entry)(is, params);
      if (s)
      return std::move(ptr);
      else
      continue;
    }
  } return exchange();
}

// default loaders
// extending:
//   provide format in {enum}
//   define corresponding validate/read/write functions in table
enum class image_format { jpeg, png, bmp, pam, hdr }; using image_bridge = bridge<ndarray<uint8_t>, image_format>;
enum class audio_format { flac, mp3, ogg, wav };      using audio_bridge = bridge<ndarray<int16_t>, audio_format>;
enum class video_format { x264 };

// custom loaders
struct 
mesh_params
{};
enum class mesh_format { fbx, dae, stl, obj };
using mesh_bridge =
  bridge
  < mesh_s,
    mesh_format,
    mesh_params,
    std::vector
    < std::unique_ptr<mesh_s>
    >
  >;

} // namespace ig

#endif // IG_SIMULATION_BRIDGE_H
