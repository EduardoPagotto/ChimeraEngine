#include "chimera/render/2d/Label.hpp"
#include "chimera/core/visible/FontManager.hpp"

namespace Chimera {

Label::Label(const std::string& text, float x, float y, const std::string& font, const glm::vec4& color)
    : Renderable2D(glm::vec3(x, y, 0.0), glm::vec2(0.0f), color), text(text), font(FontManager::get(font)) {}

Label::Label(const std::string& text, float x, float y, const glm::vec4& color)
    : Renderable2D(glm::vec3(x, y, 0.0), glm::vec2(0.0f), color), text(text), font(FontManager::get()) {}

Label::~Label() {}

void Label::submit(IRenderer2D& renderer) {
    // TODO: implementar normalizacao 2d 3d chamada de desenho
    renderer.drawString(font, text, prop2d.position, prop2d.color); // passar o obj Prop2D
}

} // namespace Chimera