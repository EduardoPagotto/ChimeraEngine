#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/core/space/Octree.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace Chimera {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d(const bool& logData);
    virtual ~Renderer3d();
    virtual void begin(Camera* camera, ViewProjection* vpo, Octree* octree) override;
    virtual bool submit(const RenderCommand& command, IRenderable3d* renderable, const bool& isSubDraw) override;
    virtual void end() override;
    virtual void flush() override;
    virtual inline std::vector<Texture*>& texQueue() { return textureQueue; }

  private:
    std::vector<RenderCommand> vRenderCommand;
    std::vector<Renderable3D*> vRenderable;
    std::vector<Texture*> textureQueue;
    Frustum frustum;
    uint32_t totIBO, totFaces;
    bool logData;
    Octree* octree;
};
} // namespace Chimera
