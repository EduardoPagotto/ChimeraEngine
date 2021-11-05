#pragma once

#include "chimera/render/partition/Plane.hpp"
#include "chimera/render/partition/Triangle.hpp"

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
