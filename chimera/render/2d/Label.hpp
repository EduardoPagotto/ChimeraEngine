#pragma once
#include "Renderable2D.hpp"
#include "chimera/core/gl/Font.hpp"

namespace ce {

class Label : public Renderable2D {

  public:
    Label(const std::string& text, float x, float y, std::shared_ptr<Font> font, const glm::vec4& color)
        : Renderable2D(glm::vec3(x, y, 0.0), glm::vec2(0.0f), color), text(text), font(font) {}

    virtual ~Label() = default;
    virtual void submit(IRenderer2D& renderer) override {
        // TODO: implementar normalizacao 2d 3d chamada de desenho
        renderer.drawString(font, text, prop2d.position, prop2d.color); // passar o obj Prop2D
    }
    inline void setText(const std::string& text) { this->text = text; }

  private:
    std::string text;
    std::shared_ptr<Font> font;
};
} // namespace ce
