#ifndef __CHIMERA_BSPTREERENDER_HPP
#define __CHIMERA_BSPTREERENDER_HPP

#include "BSPTreeNode.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/vbs/VertexNode.hpp"
#include <list>
#include <vector>

namespace Chimera {

class BspTreeRender {
  public:
    BspTreeRender(BSPTreeNode* root, std::vector<VertexNode*>& vpLeafData, std::vector<VertexData>& vertexData);
    virtual ~BspTreeRender();
    void render(glm::vec3* eye, Frustum& _frustrun, bool _logData);

  private:
    void destroy();
    void collapse(BSPTreeNode* tree);

    void traverseTree(BSPTreeNode* tree, glm::vec3* pos, Frustum& _frustrun);
    void drawPolygon(BSPTreeNode* tree, bool frontSide, Frustum& _frustrun);

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    BSPTreeNode* root;
    Core::VertexArray* vao;
    bool logdata;
    std::vector<VertexNode*> vpLeaf;
    std::vector<VertexData> vVertex;
};

} // namespace Chimera
#endif