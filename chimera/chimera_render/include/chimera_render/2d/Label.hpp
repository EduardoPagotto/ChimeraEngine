#pragma once
#include "Renderable2D.hpp"
#include "chimera_core/gl/Font.hpp"

namespace ce {

    class Label : public Renderable2D {
      public:
        Label(const std::string& text, float x, float y, std::shared_ptr<Font> font, const glm::vec4& color)
            : Renderable2D(glm::vec3(x, y, 0.0), glm::vec2(0.0F), color), text_(text), font_(font) {}

        virtual ~Label() = default;

        virtual void submit(IRenderer2D& renderer) override {
            // TODO: implementar normalizacao 2d 3d chamada de desenho
            renderer.draw_string(font_, text_, prop2d.position, prop2d.color); // passar o obj Prop2D
        }

        void set_text(const std::string& text) { this->text_ = text; }

      private:
        std::string text_;
        std::shared_ptr<Font> font_;
    };
} // namespace ce
