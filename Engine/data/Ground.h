#ifndef _GROUND_H_
#define _GROUND_H_

#include "Block.h"

namespace engine {
namespace data {

class Ground : public Block {
public:
    typedef std::shared_ptr<Block> Ptr;
    virtual ~Ground();

    inline int GetGrType() { return grType; }

protected:
    Ground(int, int, int);

protected:
    int grType;
};

}
}

#endif // _GROUND_H_
