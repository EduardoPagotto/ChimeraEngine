#pragma once
#include "Renderable2D.hpp"

namespace ce {

    class Sprite : public Renderable2D {

      public:
        Sprite(float x, float y, float width, float height, glm::vec4 color)
            : Renderable2D(glm::vec3(x, y, 0), glm::vec2(width, height), color) {}

        Sprite(float x, float y, float width, float height, std::shared_ptr<Texture> texture)
            : Renderable2D(glm::vec3(x, y, 0), glm::vec2(width, height), texture) {}

        virtual ~Sprite() {}
    };
} // namespace ce
