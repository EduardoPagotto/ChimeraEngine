#pragma once
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
    Renderer3d();
    virtual ~Renderer3d();
    virtual void begin(const glm::mat4& inverseViewProjection, const bool& logData) override;
    virtual void submit(const RenderCommand& command) override;
    virtual void end() override;
    virtual void flush() override;
    virtual inline MapUniform& uQueue() override { return uniformsQueue; }
    inline virtual TransformationStack& getStack() override { return stack; };

  private:
    std::deque<RenderCommand> commandQueue;
    MapUniform uniformsQueue;
    Frustum frustum;
    uint32_t totIBO, totFaces;
    bool logData;
    TransformationStack stack; // TODO: implementar a hierarquia de modelos direta (sem fisica)
};
} // namespace Chimera
