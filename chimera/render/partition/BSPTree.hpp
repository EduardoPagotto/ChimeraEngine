#pragma once
#include "chimera/core/space/BSPTreeNode.hpp"
#include "chimera/core/space/Triangle.hpp"
#include "chimera/core/space/TrisIndex.hpp"
#include "chimera/render/VertexData.hpp"
#include <list>

namespace Chimera {
class BspTree { // Ref: https://github.com/taylorstine/BSP_Tree
  public:
    BspTree() {}
    virtual ~BspTree() {}
    BSPTreeNode* create(Mesh* mesh, std::vector<TrisIndex>& vpLeafOut);

  private:
    BSPTreeNode* build(std::list<Triangle*>& _vTriangle);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);
    void createLeafy(BSPTreeNode* tree, std::list<Triangle*>& _vTriangle);
    void addVertexMesh(const glm::vec3& point, const glm::vec3& normal, const glm::vec2& uv);
    // bool tringleListIsConvex(std::vector<Triangle*>& _vTriangle);
    void addIndexMesh(const uint32_t& p);
    void addIndex(const glm::ivec3& i, const glm::vec3& normal, bool splitter, std::list<Triangle*>* _vTriangle);

    Mesh* mesh;
    std::vector<TrisIndex> vpLeaf;
};
} // namespace Chimera
