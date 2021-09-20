#include "chimera/render/vbs/Renderable2D.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Renderable2D::Renderable2D(glm::vec3 pos, glm::vec2 size, glm::vec4 color) : pos(pos), size(size), color(color) {}

Renderable2D::~Renderable2D() {}

} // namespace Chimera