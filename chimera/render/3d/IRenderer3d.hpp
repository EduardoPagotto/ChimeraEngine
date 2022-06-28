#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
namespace Chimera {
class IRenderer3d {
  public:
    virtual void begin(const glm::mat4& inverseViewProjection, const bool& logData) = 0;
    virtual bool submit(const class RenderCommand& command, class IRenderable3d* renderable) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual std::unordered_map<std::string, class UValue>& uQueue() = 0;
    virtual class TransformationStack& getStack() = 0;
};
} // namespace Chimera
