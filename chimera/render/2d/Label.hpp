#pragma once
#include "Renderable2D.hpp"
#include "chimera/core/visible/FontAtlas.hpp"

namespace Chimera {

class Label : public Renderable2D {

  public:
    Label(const std::string& text, float x, float y, const std::string& font, const glm::vec4& color);
    Label(const std::string& text, float x, float y, const glm::vec4& color);
    virtual ~Label();
    virtual void submit(IRenderer2D* renderer);
    inline void setText(const std::string& text) { this->text = text; }

  private:
    std::string text;
    FontAtlas* font;
};
} // namespace Chimera
