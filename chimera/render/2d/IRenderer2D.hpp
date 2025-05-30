#pragma once
#include "chimera/base/TransformationStack.hpp"
#include "chimera/core/gl/Font.hpp"
#include <string>

namespace ce {
    class IRenderer2D {
      public:
        virtual void begin(class Camera* camera) = 0;
        virtual void submit(class IRenderable2D* renderable) = 0;
        virtual void end() = 0;
        virtual void flush() = 0;
        virtual TransformationStack& getStack() = 0;
        virtual void drawString(std::shared_ptr<Font> font, const std::string& text, const glm::vec3& pos,
                                const glm::vec4& color) = 0;
        virtual void setCommandRender(struct RenderCommand* command) = 0;
    };
} // namespace ce
