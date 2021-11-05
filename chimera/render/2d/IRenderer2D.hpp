#pragma once

#include "chimera/render/FontAtlas.hpp"
#include "chimera/render/TransformationStack.hpp"
#include <string>

namespace Chimera {
class IRenderer2D {
  public:
    virtual void begin(class CameraOrthographic* camera) = 0;
    virtual void submit(class IRenderable2D* renderable) = 0;
    virtual void end() = 0;
    virtual void flush() = 0;
    virtual TransformationStack& getStack() = 0;
    virtual void drawString(FontAtlas* font, const std::string& text, const glm::vec3& pos, const glm::vec4& color) = 0;
};
} // namespace Chimera
