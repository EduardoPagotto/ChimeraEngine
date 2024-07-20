#pragma once
#include "chimera/core/TransformationStack.hpp"
#include "chimera/core/visible/Font.hpp"
#include <string>

namespace Chimera {
class IRenderer2D {
  public:
    virtual void begin(class Camera* camera) = 0;
    virtual void submit(class IRenderable2D* renderable) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual TransformationStack& getStack() = 0;
    virtual void drawString(Font* font, const std::string& text, const glm::vec3& pos, const glm::vec4& color) = 0;
    virtual void setCommandRender(class RenderCommand* command) = 0;
};
} // namespace Chimera
