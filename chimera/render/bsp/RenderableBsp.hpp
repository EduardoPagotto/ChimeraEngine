#ifndef __CHIMERA_BSPTREERENDER_HPP
#define __CHIMERA_BSPTREERENDER_HPP

#include "BSPTreeNode.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/vbs/VertexNode.hpp"
#include <deque>
#include <list>
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable {
  public:
    RenderableBsp(BSPTreeNode* root, std::vector<VertexNode*>& vpLeafData, std::vector<VertexData>& vertexData);
    virtual ~RenderableBsp();
    virtual void inject(glm::vec3* eye, Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue);
    virtual void debugDados() {}
    virtual uint32_t getSize() const { return totIndex; }
    virtual Shader* getShader() const { return nullptr; }
    virtual Core::VertexArray* getVao() const { return vao; }
    virtual Core::IndexBuffer* getIBO() const { return 0; }
    virtual AABB* getAABB() { return &aabb; }

  private:
    void destroy();
    void collapse(BSPTreeNode* tree);

    void traverseTree(BSPTreeNode* tree);
    void drawPolygon(BSPTreeNode* tree, bool frontSide);

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    BSPTreeNode* root;
    Core::VertexArray* vao;
    bool logdata;
    std::vector<VertexNode*> vpLeaf;
    std::vector<VertexData> vVertex;

    AABB aabb;
    Frustum* frustum;
    glm::vec3* eye;
    uint32_t totIndex;
    std::deque<IRenderable*>* renderQueue;
};

} // namespace Chimera
#endif