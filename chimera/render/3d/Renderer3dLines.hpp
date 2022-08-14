#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/DrawLine.hpp"
#include "chimera/core/space/Frustum.hpp"
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

    bool valid() { return drawLine.valid(); }
    void create(const uint32_t& sizeBuffer);
    void destroy();

  private:
    DrawLine drawLine;
    Frustum frustum;
};

} // namespace Chimera