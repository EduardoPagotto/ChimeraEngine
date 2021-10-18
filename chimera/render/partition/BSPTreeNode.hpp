#ifndef __CHIMERA_BSPTREE_NODE_HPP
#define __CHIMERA_BSPTREE_NODE_HPP

#include "chimera/core/space/Plane.hpp"
#include "chimera/core/space/Triangle.hpp"

namespace Chimera {

struct BSPTreeNode {
    BSPTreeNode(const Plane& _hyperPlane)
        : hyperPlane(_hyperPlane), front(nullptr), back(nullptr), leafIndex(0), isLeaf(false), isSolid(false) {}
    Plane hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    uint32_t leafIndex;
    bool isSolid;
    bool isLeaf;
};
} // namespace Chimera
#endif