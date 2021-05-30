#ifndef __CHIMERA_BSPTREE_LEAF_HPP
#define __CHIMERA_BSPTREE_LEAF_HPP

#include <list>
#include <stdint.h>

namespace Chimera {

// // TODO: classe sera subistituida por VertexNode (integrada com EBO)
class Leaf { // each leaf to have a maximum of 50 portals which is way to many but lets not
  public:
    Leaf();
    virtual ~Leaf();
    void addFace(uint32_t face, uint32_t _a, uint32_t _b, uint32_t _c);
    std::list<uint32_t> index;
    std::list<uint32_t> faces;
    // AABB boundingBox;
};
} // namespace Chimera
#endif