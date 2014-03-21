#include "config.h"
#include "Block.h"

namespace engine {
namespace data {

Block::Block(int t, int x, int y): type(t), offX(x), offY(y) {
}

Block::~Block() {
}

}
}
