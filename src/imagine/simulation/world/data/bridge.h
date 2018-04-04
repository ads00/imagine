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

#include "imagine/math/geometry/modeling/mesh.h"

#include <fstream>
#include <vector>

namespace ig {

template
< typename T,
  typename Format,
  typename Container = std::unique_ptr<T>,
  typename... Params >
class bridge {
public:
  using resource = T;
  using exchange = Container;
  using parameters = std::tuple<Params...>;
  using data       = std::pair <bool, exchange>;

  using validate = bool(*)(std::istream&);
  using read     = data(*)(std::istream&, const parameters&);
  using write    = bool(*)(std::ostream&, const parameters&, const resource&);

  template <Format fmt>
  static bool save(std::ostream& out, const resource& data,    Params&&... args);
  static auto load(std::istream& in,  const std::string& name, Params&&... args);

  static auto file_load(const std::string& filename, Params&&... args) {
    std::ifstream in{filename, std::ios::binary};
    return load(
      in,
      filename, args...);
  }

private:
  using loader =
    std::tuple
      < validate,
        read,
        write >;
  using table = std::vector<loader>;
  static IG_API table& get();
};

template
< typename T,
  typename Format,
  typename Container, typename... Params >
template <Format fmt>
bool bridge<T, Format, Container, Params...>::save(std::ostream& out, const resource& data, Params&&... args) {
  if (!out.good()) {
    throw std::runtime_error{
      "[Bridge]: Invalid output stream "
      "(inexistant, unavailable or inaccessible virtual resource)"};
  }

  auto& write = std::get<2>(get()[static_cast<size_t>(fmt)]);
  return out.good()
    ? write(out, std::tuple<Params...>{args...}, data)
    : false;
}

template
< typename T,
  typename Format,
  typename Container, typename... Params >
auto bridge<T, Format, Container, Params...>::load(std::istream& in, const std::string& name, Params&&... args) {
  assert(!name.empty() && "Invalid resource name, failing search management and loader validation");
  if (!in.good()) {
    throw std::runtime_error{
      "[Bridge] Invalid input stream "
      "(inexistant, unavailable or inaccessible virtual resource)"};
  }

  for (auto& entry : get()) {
    auto& validate = std::get<0>(entry);
    if (!validate) continue;

    if (validate(in)) {
      auto&& [s, ptr] = std::get<1>(entry)(in, std::tuple<Params...>{args...});
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
enum class mesh_format { fbx, dae, stl, obj };
using mesh_bridge =
  bridge
  < mesh_s,
    mesh_format,
    std::vector
    < std::unique_ptr<mesh_s>
    >
  >;

} // namespace ig

#endif // IG_SIMULATION_BRIDGE_H
