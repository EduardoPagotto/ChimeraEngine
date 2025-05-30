#pragma once
#include "IRenderable2D.hpp"
#include "IRenderer2D.hpp"
#include "chimera/core/gl/Texture.hpp"
#include "chimera/render/2d/Prop2D.hpp"

namespace ce {

    class Renderable2D : public IRenderable2D {

      protected:
        Prop2D prop2d;

      public:
        Renderable2D(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
            prop2d.position = pos;
            prop2d.size = size;
            prop2d.color = color;
            prop2d.texture = nullptr;
            prop2d.uv = Renderable2D::getDefaultUVs();
        }

        Renderable2D(const glm::vec3& pos, const glm::vec2& size, std::shared_ptr<Texture> texture) {
            prop2d.position = pos;
            prop2d.size = size;
            prop2d.color = glm::vec4(1, 1, 1, 1);
            prop2d.texture = texture;
            prop2d.uv = Renderable2D::getDefaultUVs();
        }

        virtual ~Renderable2D() {}
        virtual void submit(IRenderer2D& renderer) override { renderer.submit(this); }

        static const std::vector<glm::vec2>& getDefaultUVs() {
            static std::vector<glm::vec2> results;
            if (!results.size()) {
                results.push_back(glm::vec2(0, 1));
                results.push_back(glm::vec2(1, 1));
                results.push_back(glm::vec2(1, 0));
                results.push_back(glm::vec2(0, 0));
            }
            return results;
        }

        const Prop2D& getProp() const { return prop2d; }
    };
} // namespace ce
