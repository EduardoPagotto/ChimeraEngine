#pragma once

// Ref: https://github.com/taylorstine/BSP_Tree
#include "BSPTreeNode.hpp"
#include "chimera/render/3d/RenderableBsp.hpp"

namespace Chimera {

class BspTree {
  public:
    BspTree() : root(nullptr) {}
    virtual ~BspTree() {}
    // RenderableBsp* create(std::vector<VertexData>& _vVertex, std::vector<uint32_t>& _vIndex);
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
