#pragma once
#include "BSPTreeNode.hpp"
#include "chimera/core/space/Triangle.hpp"
#include "chimera/core/space/TrisIndex.hpp"
#include "chimera/render/VertexData.hpp"
#include <list>

namespace Chimera {
class BspTree { // Ref: https://github.com/taylorstine/BSP_Tree
  public:
    BspTree() {}
    virtual ~BspTree() {}
    BSPTreeNode* create(std::vector<VertexData>& _vVertexIn, std::list<Triangle*>& _vTriangleIn, std::vector<VertexData>& _vVertexOut,
                        VecPrtTrisIndex& vpLeafOut);

  private:
    BSPTreeNode* build(std::list<Triangle*>& _vTriangle);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);
    void createLeafy(BSPTreeNode* tree, std::list<Triangle*>& _vTriangle);

    std::vector<VertexData> vVertex;
    VecPrtTrisIndex vpLeaf;
};
} // namespace Chimera
