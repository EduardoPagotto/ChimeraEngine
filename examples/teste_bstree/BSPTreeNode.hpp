#ifndef __BSPTREE_NODE__HPP
#define __BSPTREE_NODE__HPP

//#include <vector>
#include "ListPolygon.hpp"
#include "Polygon.hpp"

enum SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

// struct HyperPlane {
//     std::vector<float> coefficients;
// };

struct BSPTreeNode {
    ListPolygon polygons;
    Triangle partition; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
};

#endif