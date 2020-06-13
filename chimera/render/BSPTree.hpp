#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "Triangle.hpp"
#include <vector>

namespace Chimera {

struct BSPTreeNode {
    BSPTreeNode(Triangle* partition) : front(nullptr), back(nullptr), isLeaf(false), isSolid(false) {
        hyperPlane.set(partition->vertex[0].position, partition->normal());
    }

    std::vector<Triangle> polygons;
    PlanePoint hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    bool isLeaf;
    bool isSolid;
};

//----- Build bsptree
BSPTreeNode* bsptreeBuild(std::vector<Triangle>* _pListPolygon);

//----- parse bsptree to draw
void bsptreeDraw(BSPTreeNode* _pRoot, glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool logdata);

//---- util
unsigned int selectBestSplitter(std::vector<Triangle>& _poliyList);

void initPolygons(unsigned char* map, std::vector<Triangle>* PolygonList);

} // namespace Chimera
#endif
