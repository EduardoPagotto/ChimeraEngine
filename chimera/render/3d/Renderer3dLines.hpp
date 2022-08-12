#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/DrawLine.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace Chimera {

class Renderer3dLines : public IRenderer3d {
  public:
    Renderer3dLines();
    virtual ~Renderer3dLines();
    virtual void begin(Camera* camera, ViewProjection* vpo, Octree* octree) override;
    virtual bool submit(const RenderCommand& command, IRenderable3d* renderable) override;
    virtual void end() override;
    virtual void flush() override;

    void create(const uint32_t& sizeBuffer);
    void destroy();

  private:
    void add(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& color) {
        points.push_back({p0, color});
        points.push_back({p1, color});
    }
    void addAABB(const AABB& aabb, const glm::vec3& color);

    RenderCommand command;
    Shader shader;
    std::vector<LinesValues> points;
    VertexArray* pVao;
    VertexBuffer* pVbo;
    Frustum frustum;
};

} // namespace Chimera