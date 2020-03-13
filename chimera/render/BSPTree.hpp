#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "Triangle.hpp"
#include <vector>

namespace Chimera {

struct BSPTreeNode {
    BSPTreeNode(Triangle* partition)
        : hyperPlane(new PlanePoint(partition)), front(nullptr), back(nullptr), isLeaf(false), isSolid(false) {}
    std::vector<Chimera::Triangle> polygons;
    PlanePoint* hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    bool isLeaf;
    bool isSolid;
};

//----- Build bsptree
BSPTreeNode* bsptreeBuild(std::vector<Chimera::Triangle>* _pListPolygon);

//----- parse bsptree to draw
void bsptreeDraw(BSPTreeNode* _pRoot, glm::vec3* eye, std::vector<Chimera::VertexData>* _pOutVertex, bool logdata);

//---- util
unsigned int selectBestSplitter(std::vector<Chimera::Triangle>& _poliyList);

void initPolygons(unsigned char* map, std::vector<Chimera::Triangle>* PolygonList);

} // namespace Chimera
#endif
