#ifndef __CHIMERA_RENDERABLE_BSP_HPP
#define __CHIMERA_RENDERABLE_BSP_HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/space/BSPTreeNode.hpp"
#include "chimera/render/3d/RenderableFace.hpp"
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable3d {
  public:
    RenderableBsp(Entity entity, BSPTreeNode* root, std::vector<RenderableFace*>* vpLeafData, std::vector<VertexData>* vertexData);
    virtual ~RenderableBsp();
    virtual void debugDados() override;
    virtual uint32_t getSize() const { return totIndex; }
    virtual Shader* getShader() const { return nullptr; }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return 0; }
    virtual AABB* getAABB() { return &aabb; }
    virtual void submit(Renderer3d* renderer) override;
    virtual Entity getEntity() const override { return entity; }

    void setEyePosition(glm::vec3* eye) { this->eye = eye; }

  private:
    void destroy();
    void collapse(BSPTreeNode* tree);

    void traverseTree(BSPTreeNode* tree);
    void drawPolygon(BSPTreeNode* tree, bool frontSide);

    // TODO: Testar!!!!!!
    bool lineOfSight(glm::vec3* Start, glm::vec3* End, BSPTreeNode* tree);

    BSPTreeNode* root;
    VertexArray* vao;
    bool logdata;
    std::vector<RenderableFace*> vpLeaf;
    std::vector<VertexData> vVertex;

    AABB aabb;
    glm::vec3* eye;
    uint32_t totIndex;
    Renderer3d* renderer;
    Entity entity;
};

} // namespace Chimera
#endif