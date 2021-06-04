#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree

#include "BSPTreeNode.hpp"
#include "chimera/render/vbs/VertexData.hpp"
#include <list>
#include <vector>

namespace Chimera {

class BspTree {
  public:
    BspTree();
    virtual ~BspTree();
    void create(std::vector<Chimera::VertexData>& _vVertex, const std::vector<unsigned int>& _vIndex);
    void destroy();

    void render(glm::vec3* eye, std::vector<VertexData>* _pOutVertex, bool _logData);

  private:
    BSPTreeNode* buildLeafy(std::list<Triangle*>& _vTriangle);

    void drawPolygon(BSPTreeNode* tree, bool frontSide);
    void traverseTree(BSPTreeNode* tree, glm::vec3* pos);

    void collapse(BSPTreeNode* tree);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    bool logdata;
    BSPTreeNode* root;

    std::vector<Chimera::VertexData> vVertex;
    std::vector<VertexData>* resultVertex;
};

} // namespace Chimera
#endif
