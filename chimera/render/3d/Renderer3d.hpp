#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/TransformationStack.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/core/space/Octree.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d(const bool& logData);
    virtual ~Renderer3d();
    virtual void begin(Camera* camera, ViewProjection* vpo, Octree* octree) override;
    virtual bool submit(const RenderCommand& command, IRenderable3d* renderable) override;
    virtual void end() override;
    virtual void flush() override;
    virtual inline TransformationStack& getStack() override { return stack; };
    virtual inline MapUniform& uboQueue() override { return uniformsQueue; }
    virtual inline std::vector<Texture*>& texQueue() override { return textureQueue; }
    virtual inline Camera* getCamera() const override { return camera; }
    virtual inline ViewProjection* getEyeView() const override { return vpo; }

  private:
    Camera* camera;
    ViewProjection* vpo;
    std::vector<std::tuple<RenderCommand, Renderable3D*>> renderQueue;
    std::vector<Texture*> textureQueue;
    MapUniform uniformsQueue;
    Frustum frustum;
    uint32_t totIBO, totFaces;
    bool logData;
    TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
    Octree* octree;
};
} // namespace Chimera
