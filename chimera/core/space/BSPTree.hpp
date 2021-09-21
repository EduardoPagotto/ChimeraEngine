#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref: https://github.com/taylorstine/BSP_Tree
#include "BSPTreeNode.hpp"
#include "chimera/render/vbs/RenderableBsp.hpp"

namespace Chimera::Core {

class BspTree {
  public:
    BspTree() : root(nullptr) {}
    virtual ~BspTree() {}
    // RenderableBsp* create(std::vector<Chimera::VertexData>& _vVertex, std::vector<uint32_t>& _vIndex);
    void create(std::vector<Chimera::VertexData>& _vVertex, std::vector<uint32_t>& _vIndex);

    BSPTreeNode* getRoot() const { return root; }
    std::vector<Chimera::VertexData>* getVertex() { return &vVertex; }
    std::vector<RenderableIBO*>* getLeafs() { return &vpLeaf; }

  private:
    BSPTreeNode* build(std::list<Triangle*>& _vTriangle);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);
    void createLeafy(BSPTreeNode* tree, std::list<Triangle*>& _vTriangle);

    BSPTreeNode* root;
    std::vector<Chimera::VertexData> vVertex;
    std::vector<RenderableIBO*> vpLeaf;
};
} // namespace Chimera::Core
#endif