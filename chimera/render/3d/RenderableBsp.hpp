#pragma once
#include "RenderCommand.hpp"
#include "Renderable3D.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/BSPTreeNode.hpp"
#include <vector>

namespace Chimera {

class RenderableBsp : public IRenderable3d {
  public:
    RenderableBsp(BSPTreeNode* root, std::vector<Renderable3D*>* vChild, std::vector<VertexData>* vertexData);
    virtual ~RenderableBsp();
    virtual uint32_t getSize() const { return totIndex; }
    virtual VertexArray* getVao() const { return vao; }
    virtual IndexBuffer* getIBO() const { return nullptr; }
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
