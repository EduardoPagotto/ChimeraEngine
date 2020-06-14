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
    void render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData);

  private:
    unsigned int selectBestSplitter(std::vector<Triangle>& _poliyList);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane* hyperPlane, std::vector<Triangle>* _pListPolygon);
    BSPTreeNode* bsptreeBuild(std::vector<Triangle>* _pListPolygon);

    void drawPolygon(BSPTreeNode* tree, bool frontSide);
    void traverseTree(BSPTreeNode* tree, glm::vec3* pos);

    // TODO: continuar a implementar abaixo
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    bool logdata;
    BSPTreeNode* root;
    std::vector<VertexData>* resultVertex;
};
} // namespace Chimera
#endif
