#include "chimera/render/2d/Label.hpp"

namespace Chimera {

Label::Label(const std::string& text, float x, float y, const glm::vec4& color)
    : Renderable2D(glm::vec3(x, y, 0.0), glm::vec2(0.0f), color), text(text) {}

Label::~Label() {}

void Label::submit(IRenderer2D* renderer) {
    // TODO
    renderer->drawString(text, pos, color);
}

} // namespace Chimera