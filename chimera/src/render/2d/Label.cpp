#include "chimera/render/2d/Label.hpp"
#include "chimera/render/FontManager.hpp"

namespace Chimera {

Label::Label(const std::string& text, float x, float y, const std::string& font, const glm::vec4& color)
    : Renderable2D(glm::vec3(x, y, 0.0), glm::vec2(0.0f), color), text(text), font(FontManager::get(font)) {}

Label::~Label() {}

void Label::submit(IRenderer2D* renderer) {
    // TODO
    renderer->drawString(font, text, pos, color);
}

} // namespace Chimera