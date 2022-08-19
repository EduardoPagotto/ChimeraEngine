#pragma once
#include "chimera/core/TransformationStack.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/UValue.hpp"
#include <vector>

namespace Chimera {
class IRenderer3d {
  public:
    IRenderer3d() { uniformsQueue.reserve(500); }
    virtual ~IRenderer3d() = default;
    virtual void begin(class Camera* camera, class ViewProjection* vpo, class Octree* octree) = 0;
    virtual bool submit(const class RenderCommand& command, class IRenderable3d* renderable, const bool& isSubDraw) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;

    inline Camera* getCamera() const { return camera; }
    inline ViewProjection* getViewProjection() const { return vpo; }
    inline TransformationStack& getStack() { return stack; };
    inline MapUniform& uboQueue() { return uniformsQueue; }

  protected:
    Camera* camera = nullptr;
    ViewProjection* vpo = nullptr;
    TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
    MapUniform uniformsQueue;
};
} // namespace Chimera
