#ifndef __CHIMERA_RENDERABLE_BSP_HPP
#define __CHIMERA_RENDERABLE_BSP_HPP

#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/core/space/BSPTreeNode.hpp"
#include "chimera/render/vbs/RenderableIBO.hpp"
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable {
  public:
    RenderableBsp(Core::BSPTreeNode* root, std::vector<RenderableIBO*>* vpLeafData, std::vector<VertexData>* vertexData);
    virtual ~RenderableBsp();
    virtual void debugDados() override;
    virtual uint32_t getSize() const { return totIndex; }
    virtual Shader* getShader() const { return nullptr; }
    virtual Core::VertexArray* getVao() const { return vao; }
    virtual Core::IndexBuffer* getIBO() const { return 0; }
    virtual Core::AABB* getAABB() { return &aabb; }
    virtual void submit(IRenderer3d* renderer) override;

    void setEyePosition(glm::vec3* eye) { this->eye = eye; }

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
    glm::vec3* eye;
    uint32_t totIndex;
    IRenderer3d* renderer;
};

} // namespace Chimera
#endif