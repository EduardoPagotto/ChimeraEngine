#pragma once
#include "BSPTreeNode.hpp"
#include "chimera/render/3d/RenderableSimple.hpp"
#include "chimera/render/partition/Triangle.hpp"

namespace Chimera {
// Ref: https://github.com/taylorstine/BSP_Tree
class BspTree {
  public:
    BspTree() : root(nullptr) {}
    virtual ~BspTree() {}
    void create(std::vector<VertexData>& _vVertex, std::vector<uint32_t>& _vIndex);
    BSPTreeNode* getRoot() const { return root; }
    std::vector<VertexData>* getVertex() { return &vVertex; }
    std::vector<RenderableSimple*>* getLeafs() { return &vpLeaf; }

  private:
    BSPTreeNode* build(std::list<Triangle*>& _vTriangle);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);
    void createLeafy(BSPTreeNode* tree, std::list<Triangle*>& _vTriangle);

    BSPTreeNode* root;
    std::vector<VertexData> vVertex;
    std::vector<RenderableSimple*> vpLeaf;
};
} // namespace Chimera
