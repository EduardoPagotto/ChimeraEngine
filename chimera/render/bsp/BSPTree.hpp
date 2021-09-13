#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref: https://github.com/taylorstine/BSP_Tree
#include "BSPTreeNode.hpp"
#include "BspTreeRender.hpp"

namespace Chimera {

class BspTree {
  public:
    BspTree();
    virtual ~BspTree();
    BspTreeRender* create(std::vector<Chimera::VertexData>& _vVertex, std::vector<uint32_t>& _vIndex);
    void destroy();

  private:
    BSPTreeNode* buildLeafy(std::list<Triangle*>& _vTriangle);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);

    void createLeafy(BSPTreeNode* tree, std::list<Triangle*>& _vTriangle);

    // bool logdata;
    BSPTreeNode* root;
    std::vector<Chimera::VertexData> vVertex;
    std::vector<VertexNode*> vpLeaf;
};
} // namespace Chimera
#endif
