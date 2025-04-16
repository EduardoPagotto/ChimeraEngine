#pragma once
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/space/BSPTreeNode.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace ce {

class RenderableBsp : public Renderable3D {
  public:
    RenderableBsp(Mesh& mesh);
    virtual ~RenderableBsp();
    virtual const uint32_t getSize() const override { return totIndex; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;

  private:
    void destroy();
    void collapse(BSPTreeNode* tree);
    void traverseTree(const glm::vec3& cameraPos, BSPTreeNode* tree, std::vector<IRenderable3d*>& childDraw);
    // TODO: Testar!!!!!!
    bool lineOfSight(const glm::vec3& Start, const glm::vec3& End, BSPTreeNode* tree);

    std::vector<IRenderable3d*> vChild;
    AABB aabb;
    uint32_t totIndex;
    BSPTreeNode* root;
};
} // namespace ce
