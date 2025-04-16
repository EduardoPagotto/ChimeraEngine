#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/core/space/Octree.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace ce {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d(const bool& logData);
    virtual ~Renderer3d();
    virtual void begin(Camera* camera, IViewProjection* vpo, Octree* octree) override;
    virtual void submit(const RenderCommand& command, IRenderable3d* renderable, const uint32_t& count) override;
    virtual void end() override;
    virtual void flush() override;
    virtual inline std::vector<Texture*>& texQueue() { return textureQueue; }

  private:
    std::queue<uint32_t> qRenderableIndexes;
    std::vector<RenderCommand> vRenderCommand;
    std::vector<Renderable3D*> vRenderable;
    std::vector<Texture*> textureQueue;
    Frustum frustum;
    bool logData;
    Octree* octree;
};
} // namespace ce
