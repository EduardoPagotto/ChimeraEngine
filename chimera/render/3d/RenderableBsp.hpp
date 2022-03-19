#pragma once
#include "RenderCommand.hpp"
#include "Renderable3D.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/TrisIndex.hpp"
#include "chimera/render/VertexData.hpp"
#include "chimera/render/partition/BSPTreeNode.hpp"
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable3d {
  public:
    RenderableBsp(BSPTreeNode* root, VecPrtTrisIndex& vTris, std::vector<VertexData>& vertexData);
    virtual ~RenderableBsp();
    virtual const uint32_t getSize() const override { return totIndex; }
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void draw(const bool& logData) override;

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
    std::vector<Renderable3D*> vChild;
    std::vector<VertexData> vVertex;
    AABB aabb;
    ICamera* camera;
    uint32_t totIndex;
    IRenderer3d* renderer;
    RenderCommand* command;
};
} // namespace Chimera
