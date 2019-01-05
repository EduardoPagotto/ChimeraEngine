#ifndef __BSPTREE_NODE__HPP
#define __BSPTREE_NODE__HPP

//#include <vector>
#include "ArrayTriangle.hpp"

enum class SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

// struct HyperPlane {
//     std::vector<float> coefficients;
// };

struct BSPTreeNode {
    ArrayTriangle arrayTriangle;
    Triangle partition; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
};

#endif