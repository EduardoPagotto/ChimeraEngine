#pragma once
#include "chimera/core/TransformationStack.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/UValue.hpp"
#include <vector>
namespace Chimera {
class IRenderer3d {
  public:
    virtual void begin(class Camera* camera, class ViewProjection* vpo, class Octree* octree) = 0;
    virtual bool submit(const class RenderCommand& command, class IRenderable3d* renderable) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual MapUniform& uboQueue() = 0;
    virtual std::vector<class Texture*>& texQueue() = 0;

    inline Camera* getCamera() const { return camera; }
    inline ViewProjection* getViewProjection() const { return vpo; }
    inline TransformationStack& getStack() { return stack; };

  protected:
    Camera* camera;
    ViewProjection* vpo;
    TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
};
} // namespace Chimera
