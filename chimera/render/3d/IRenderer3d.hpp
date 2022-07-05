#pragma once
#include "chimera/core/visible/UValue.hpp"
namespace Chimera {
class IRenderer3d {
  public:
    virtual void begin(const glm::mat4& inverseViewProjection) = 0;
    virtual bool submit(const class RenderCommand& command, class IRenderable3d* renderable) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual MapUniform& uboQueue() = 0;
    virtual class TransformationStack& getStack() = 0;
};
} // namespace Chimera
