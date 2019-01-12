#ifndef __BSPTREE_NODE__HPP
#define __BSPTREE_NODE__HPP

#include "Triangle.hpp"
#include <vector>

enum class SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

// struct HyperPlane {
//     std::vector<float> coefficients;
// };

struct BSPTreeNode {
    BSPTreeNode(const Triangle& _partition) : partition(_partition), front(nullptr), back(nullptr) {}
    std::vector<Triangle> arrayTriangle;
    Triangle partition; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
};

#endif