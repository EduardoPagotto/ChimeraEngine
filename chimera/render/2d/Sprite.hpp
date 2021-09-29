#ifndef __CHIMERA_SPRITE__HPP
#define __CHIMERA_SPRITE__HPP

#include "Renderable2D.hpp"

namespace Chimera {

class Sprite : public Renderable2D {
  public:
    Sprite(float x, float y, float width, float height, glm::vec4 color);
    Sprite(float x, float y, float width, float height, Texture* texture);
    virtual ~Sprite() {}
};

} // namespace Chimera
#endif