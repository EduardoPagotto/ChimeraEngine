#pragma once
#include "IRenderable3d.hpp"
#include "RenderCommand.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/core/space/BSPTreeNode.hpp"
#include "chimera/core/space/TrisIndex.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable3d {
  public:
    RenderableBsp(BSPTreeNode* root, std::vector<TrisIndex>& vTris, Mesh* mesh);
    virtual ~RenderableBsp();
    virtual const uint32_t getSize() const override { return totIndex; }
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;
    virtual void draw(const bool& logData) override;

  private:
    void destroy();
    void collapse(BSPTreeNode* tree);
    void traverseTree(const glm::vec3& cameraPos, BSPTreeNode* tree, std::vector<IRenderable3d*>& childDraw);
    // TODO: Testar!!!!!!
    bool lineOfSight(const glm::vec3& Start, const glm::vec3& End, BSPTreeNode* tree);

    VertexArray* vao;
    std::vector<IRenderable3d*> vChild;
    AABB aabb;
    uint32_t totIndex;
    BSPTreeNode* root;
};
} // namespace Chimera
