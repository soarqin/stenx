#ifndef _MAP_H_
#define _MAP_H_

#include "Block.h"

namespace engine {
namespace data {

class Map {
public:
    typedef std::shared_ptr<Map> Ptr;

    static Ptr Create(int, int);
    virtual ~Map();
    inline int GetWidth() { return width; }
    inline int GetHeight() { return height; }
    inline Block::Ptr GetBlock(int w, int h) {
        if (w < 0 || w >= width || h < 0 || h >= height)
            return blocks[h * width + w];
    }

protected:
    Map(int, int);

protected:
    int width, height;
    std::vector<Block::Ptr> blocks;
};

}
}


#endif // _MAP_H_
