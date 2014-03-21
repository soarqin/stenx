#ifndef _BLOCK_H_
#define _BLOCK_H_

namespace engine {
namespace data {

class Block {
public:
  typedef std::shared_ptr<Block> Ptr;
  virtual ~Block();

  inline int GetType() { return type; }
  inline int GetX() { return offX; }
  inline int GetY() { return offY; }
  inline void GetXY(int &x, int &y) { x = offX; y = offY; }

protected:
  Block(int, int, int);

protected:
  int type, offX, offY;
};

}
}


#endif // _BLOCK_H_
