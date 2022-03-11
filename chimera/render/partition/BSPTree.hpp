#pragma once
#include "BSPTreeNode.hpp"
#include "chimera/core/space/Triangle.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include <list>

namespace Chimera {
// Ref: https://github.com/taylorstine/BSP_Tree
class BspTree {
  public:
    BspTree() : root(nullptr) {}
    virtual ~BspTree() {}
    void create(std::vector<VertexData>& _vVertex, std::vector<uint32_t>& _vIndex);
    BSPTreeNode* getRoot() const { return root; }
    std::vector<VertexData>* getVertex() { return &vVertex; }
    std::vector<Renderable3D*>* getLeafs() { return &vpLeaf; }

  private:
    BSPTreeNode* build(std::list<Triangle*>& _vTriangle);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);
    void createLeafy(BSPTreeNode* tree, std::list<Triangle*>& _vTriangle);

    BSPTreeNode* root;
    std::vector<VertexData> vVertex;
    std::vector<Renderable3D*> vpLeaf;
};
} // namespace Chimera
