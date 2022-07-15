#pragma once
#include "chimera/core/TransformationStack.hpp"
#include "chimera/core/visible/FontAtlas.hpp"
#include <string>

namespace Chimera {
class IRenderer2D {
  public:
    virtual void begin(class Camera* camera) = 0;
    virtual void submit(class IRenderable2D* renderable, const class Prop2D& prop) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual TransformationStack& getStack() = 0;
    virtual void drawString(FontAtlas* font, const std::string& text, const glm::vec3& pos, const glm::vec4& color) = 0;
    virtual void setCommandRender(class RenderCommand* command) = 0;
};
} // namespace Chimera
