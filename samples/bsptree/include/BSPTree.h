#ifndef __BSPTREE_HPP
#define __BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "chimera/core/Triangle.hpp"

enum class SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

// struct HyperPlane {
//     std::vector<float> coefficients;
// };

struct BSPTreeNode {
    BSPTreeNode(const Chimera::Triangle& _partition) : partition(_partition), front(nullptr), back(nullptr) {}
    std::vector<Chimera::Triangle> polygons;
    Chimera::Triangle partition; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
};

//----- Build bsptree
BSPTreeNode* bsptreeBuild(std::vector<Chimera::Triangle>* _pListPolygon);

//----- parse bsptree to draw
void bsptreeDraw(BSPTreeNode* _pRoot, glm::vec3* eye, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata);

#endif
