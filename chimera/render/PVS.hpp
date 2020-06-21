#ifndef __CHIMERA_PVS_HPP
#define __CHIMERA_PVS_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "Plane.hpp"
#include "Triangle.hpp"
#include "VertexData.hpp"
#include <vector>

namespace Chimera {

struct Leaf { // each leaf to have a maximum of 50 portals which is way to many but lets not
    std::vector<Triangle*> polygons;
    AABB boundingBox
};

struct BSPTreeNode {
    BSPTreeNode(const Plane& _hyperPlane) : hyperPlane(_hyperPlane), front(nullptr), back(nullptr), isLeaf(false), isSolid(false) {}
    std::vector<Triangle*> polygons;
    Plane hyperPlane; // HyperPlane partition;
    BSPTreeNode* front;
    BSPTreeNode* back;
    bool isLeaf;
    bool isSolid;
};

class PVS {
  public:
    PVS();
    void createSequencial(std::vector<Chimera::VertexData>& _vVertex);
    void createIndexed(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex);
    void render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData);
    void destroy();

  private:
    Plane selectBestSplitter(std::vector<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::vector<Triangle*>& _vTriangle);
    BSPTreeNode* bsptreeBuild(std::vector<Triangle*>& _vTriangle);
    void collapse(BSPTreeNode* tree);

    // bool isConvex(std::vector<Triangle*>& _vTriangle, Triangle* _poly);

    void drawPolygon(BSPTreeNode* tree, bool frontSide);
    void traverseTree(BSPTreeNode* tree, glm::vec3* pos);

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    inline glm::vec3 vPosVal(Triangle* _t, const unsigned& pos) { return vVertex[_t->p[pos]].position; }
    inline VertexData vVerVal(Triangle* _t, const unsigned& pos) { return vVertex[_t->p[pos]]; }
    bool logdata;
    BSPTreeNode* root;
    std::vector<VertexData>* resultVertex;
    std::vector<Chimera::VertexData> vVertex;
};
} // namespace Chimera

#endif