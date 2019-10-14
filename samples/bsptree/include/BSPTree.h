#ifndef __BSPTREE_HPP
#define __BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "BSPTreeNode.hpp"

void drawBSPTree(BSPTreeNode* _pRoot, glm::vec3* eye, std::vector<Chimera::VertexDataFull>* _pOutVertex);

#endif
