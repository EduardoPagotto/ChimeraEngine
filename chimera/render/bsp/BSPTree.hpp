#ifndef __CHIMERA_BSPTREE_HPP
#define __CHIMERA_BSPTREE_HPP

// Ref:
// https://github.com/taylorstine/BSP_Tree
#include "BSPTreeNode.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/buffers/VertexBuffer.hpp"
#include "chimera/render/vbs/VertexNode.hpp"
#include <list>
#include <vector>

namespace Chimera {

class BspTreeRender {
    // public:
    //   BspTreeRender(BSPTreeNode* root, std::vector<VertexNode*>& vpLeafData, std::vector<VertexData>& vertexData);
    //   virtual ~BspTreeRender();
    //   void render(glm::vec3* eye, Frustum& _frustrun, bool _logData);
    //   void renderAABB();

    // private:
    //   void destroy();
    //   void collapse(BSPTreeNode* tree);

    //   void traverseTree(BSPTreeNode* tree, glm::vec3* pos, Frustum& _frustrun);
    //   void drawPolygon(BSPTreeNode* tree, bool frontSide, Frustum& _frustrun);

    BSPTreeNode* root;
    Core::VertexArray* vao;
    bool logdata;
    std::vector<VertexNode*> vpLeaf;
    std::vector<VertexData> vVertex;
};

class BspTree {
  public:
    BspTree();
    virtual ~BspTree();
    BspTreeRender* create(std::vector<Chimera::VertexData>& _vVertex, std::vector<uint32_t>& _vIndex);
    void destroy();
    void render(glm::vec3* eye, Frustum& _frustrun, bool _logData);
    void renderAABB();

  private:
    BSPTreeNode* buildLeafy(std::list<Triangle*>& _vTriangle);

    void drawPolygon(BSPTreeNode* tree, bool frontSide, Frustum& _frustrun);
    void traverseTree(BSPTreeNode* tree, glm::vec3* pos, Frustum& _frustrun);

    void collapse(BSPTreeNode* tree);
    Triangle* selectBestSplitter(std::list<Triangle*>& _vTriangle);
    void splitTriangle(const glm::vec3& fx, Triangle* _pTriangle, Plane& hyperPlane, std::list<Triangle*>& _vTriangle);

    void createLeafy(BSPTreeNode* tree, std::list<Triangle*>& _vTriangle);

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    bool logdata;
    BSPTreeNode* root;
    Core::VertexArray* vao;
    std::vector<Chimera::VertexData> vVertex;
    std::vector<VertexNode*> vpLeaf;
};

} // namespace Chimera
#endif
