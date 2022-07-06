#pragma once
#include "IRenderable3d.hpp"
#include "IRenderer3d.hpp"
#include "chimera/core/TransformationStack.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/core/visible/Shader.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d(const bool& logData);
    virtual ~Renderer3d();
    virtual void begin(Camera* camera, EyeView* eyeView) override;
    virtual bool submit(const RenderCommand& command, IRenderable3d* renderable) override;
    virtual void end() override;
    virtual void flush() override;
    virtual inline TransformationStack& getStack() override { return stack; };
    virtual inline MapUniform& uboQueue() override { return uniformsQueue; }
    virtual inline Camera* getCamera() const override { return camera; }
    virtual inline EyeView* getEyeView() const override { return eyeView; }

  private:
    Camera* camera;
    EyeView* eyeView;
    std::deque<RenderCommand> commandQueue;
    std::deque<IRenderable3d*> renderableQueue;
    MapUniform uniformsQueue;
    Frustum frustum;
    uint32_t totIBO, totFaces;
    bool logData;
    TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
};
} // namespace Chimera
