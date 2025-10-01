#pragma once
#include "IRenderable2D.hpp"
#include "IRenderer2D.hpp"

namespace ce {

    class Group : public IRenderable2D {

      private:
        std::vector<IRenderable2D*> renderables;
        glm::mat4 transformationMatrix;

      public:
        Group(const glm::mat4& transform) : transformationMatrix(transform) {}

        virtual ~Group() {}

        virtual void submit(IRenderer2D& renderer) override {

            renderer.getStack().push(transformationMatrix);
            for (auto renderable : renderables)
                renderable->submit(renderer);

            renderer.getStack().pop();
        }

        inline void add(IRenderable2D* renderable) { renderables.push_back(renderable); }
    };
} // namespace ce
