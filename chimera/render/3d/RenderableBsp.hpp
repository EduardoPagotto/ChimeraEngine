#ifndef __CHIMERA_RENDERABLE_BSP_HPP
#define __CHIMERA_RENDERABLE_BSP_HPP

#include "chimera/core/Transform.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/3d/RenderableFace.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/partition/BSPTreeNode.hpp"
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable3d {
  public:
    RenderableBsp(Entity entity, BSPTreeNode* root, std::vector<RenderableFace*>* vpLeafData, std::vector<VertexData>* vertexData);
    virtual ~RenderableBsp();
    virtual void debugDados() override;
    virtual uint32_t getSize() const { return totIndex; }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return 0; }
    virtual AABB* getAABB() { return &aabb; }
    virtual void submit(Camera* camera, IRenderer3d* renderer) override;
    virtual Entity getEntity() const override { return entity; }
    virtual Material* getMaterial() override { return material; }

  private:
    void destroy();
    void collapse(BSPTreeNode* tree);

    void traverseTree(BSPTreeNode* tree);
    void drawPolygon(BSPTreeNode* tree, bool frontSide);

    // TODO: Testar!!!!!!
    bool lineOfSight(const glm::vec3& Start, const glm::vec3& End, BSPTreeNode* tree);

    BSPTreeNode* root;
    VertexArray* vao;
    bool logdata;
    std::vector<RenderableFace*> vpLeaf;
    std::vector<VertexData> vVertex;

    AABB aabb;
    Camera* camera;
    uint32_t totIndex;
    IRenderer3d* renderer;
    Entity entity;
    Material* material;
    Shader shader;
    Transform* model;
};

} // namespace Chimera
#endif