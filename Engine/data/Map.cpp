#include "config.h"
#include "Map.h"

namespace engine {
namespace data {

Map::Ptr Map::Create(int w, int h) {
  return Ptr(new Map(w, h));
}

Map::Map(int w, int h) {
  width = w;
  height = h;
  blocks.resize(h * width + w);
}

Map::~Map() {
}

}
}
