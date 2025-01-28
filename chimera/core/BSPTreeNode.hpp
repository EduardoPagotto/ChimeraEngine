#pragma once
#include "Plane.hpp"

namespace ce {

struct BSPTreeNode {
    Plane hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    uint32_t leafIndex;
    bool isSolid;
    bool isLeaf;

    BSPTreeNode(const Plane& _hyperPlane)
        : hyperPlane(_hyperPlane), front(nullptr), back(nullptr), leafIndex(0), isLeaf(false), isSolid(false) {}
};
} // namespace ce
