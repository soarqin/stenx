#include "config.h"
#include "Ground.h"

namespace engine {
namespace data {

Ground::Ground(int t, int x, int y) : Block(0, x, y), grType(t) {
}

Ground::~Ground() {
}

}
}
