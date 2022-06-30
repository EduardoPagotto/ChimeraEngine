#pragma once
#include "IRenderable3d.hpp"
#include "IRenderer3d.hpp"
#include "RenderCommand.hpp"
#include "chimera/core/TransformationStack.hpp"
#include "chimera/core/space/Frustum.hpp"
#include "chimera/core/visible/Light.hpp"
#include "chimera/core/visible/Shader.hpp"
#include <cstdint>
#include <deque>

namespace Chimera {

class Renderer3d : public IRenderer3d {
  public:
    Renderer3d(MapUniform* ubo, const bool& logData);
    virtual ~Renderer3d();
    virtual void begin(const glm::mat4& inverseViewProjection) override;
    virtual bool submit(const RenderCommand& command, IRenderable3d* renderable) override;
    virtual void end() override;
    virtual void flush() override;
    inline virtual TransformationStack& getStack() override { return stack; };

  private:
    std::deque<RenderCommand> commandQueue;
    std::deque<IRenderable3d*> renderableQueue;
    MapUniform* ubo;
    Frustum frustum;
    uint32_t totIBO, totFaces;
    bool logData;
    TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
};
} // namespace Chimera
