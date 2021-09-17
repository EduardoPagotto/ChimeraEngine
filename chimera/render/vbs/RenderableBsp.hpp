#ifndef __CHIMERA_RENDERABLE_BSP_HPP
#define __CHIMERA_RENDERABLE_BSP_HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/space/BSPTreeNode.hpp"
#include "chimera/render/vbs/RenderableIBO.hpp"
#include <deque>
#include <list>
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable {
  public:
    RenderableBsp(Core::BSPTreeNode* root, std::vector<RenderableIBO*>* vpLeafData, std::vector<VertexData>* vertexData);
    virtual ~RenderableBsp();
    virtual void inject(glm::vec3* eye, Core::Frustum* frustum, bool logData, std::deque<IRenderable*>* renderQueue);
    virtual void debugDados() {}
    virtual uint32_t getSize() const { return totIndex; }
    virtual Shader* getShader() const { return nullptr; }
    virtual Core::VertexArray* getVao() const { return vao; }
    virtual Core::IndexBuffer* getIBO() const { return 0; }
    virtual Core::AABB* getAABB() { return &aabb; }

  private:
    void destroy();
    void collapse(Core::BSPTreeNode* tree);

    void traverseTree(Core::BSPTreeNode* tree);
    void drawPolygon(Core::BSPTreeNode* tree, bool frontSide);

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, Core::BSPTreeNode* tree);

    Core::BSPTreeNode* root;
    Core::VertexArray* vao;
    bool logdata;
    std::vector<RenderableIBO*> vpLeaf;
    std::vector<VertexData> vVertex;

    Core::AABB aabb;
    Core::Frustum* frustum;
    glm::vec3* eye;
    uint32_t totIndex;
    std::deque<IRenderable*>* renderQueue;
};

} // namespace Chimera
#endif