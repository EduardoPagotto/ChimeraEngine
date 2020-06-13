#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "Plane.hpp"
#include "Triangle.hpp"
#include <vector>

namespace Chimera {

struct BSPTreeNode {
    BSPTreeNode(const glm::vec3& _point, const glm::vec3& _normal) : front(nullptr), back(nullptr), isLeaf(false), isSolid(false) {
        hyperPlane.set(_point, _normal);
    }

    std::vector<Triangle> polygons;
    Plane hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    bool isLeaf;
    bool isSolid;
};

class BspTree {
  public:
    BspTree();
    void create(std::vector<Triangle>* _pListPolygon);
    void render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool logdata);

  private:
    BSPTreeNode* root;
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
