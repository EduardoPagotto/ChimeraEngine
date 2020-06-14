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
    void create(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex);
    void render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData);

  private:
    unsigned int selectBestSplitter(std::vector<Triangle>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle& _pTriangle, Plane& hyperPlane, std::vector<Triangle>& _vTriangle);
    BSPTreeNode* bsptreeBuild(std::vector<Triangle>& _vTriangle);

    void drawPolygon(BSPTreeNode* tree, bool frontSide);
    void traverseTree(BSPTreeNode* tree, glm::vec3* pos);

    // TODO: continuar a implementar abaixo
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    inline glm::vec3 vPosVal(const Triangle& _t, const unsigned& pos) { return (*vVertex)[_t.p[pos]].position; }
    inline VertexData vVerVal(const Triangle& _t, const unsigned& pos) { return (*vVertex)[_t.p[pos]]; }
    bool logdata;
    BSPTreeNode* root;
    std::vector<VertexData>* resultVertex;
    std::vector<Chimera::VertexData>* vVertex;
};
} // namespace Chimera
#endif
