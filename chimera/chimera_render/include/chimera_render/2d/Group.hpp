#pragma once
#include "IRenderable2D.hpp"
#include "IRenderer2D.hpp"

namespace ce {

    class Group : public IRenderable2D {
      public:
        Group(const glm::mat4& transform) : transformation_matrix_(transform) {}

        virtual ~Group() {}

        virtual void submit(IRenderer2D& renderer) override {

            renderer.get_stack().push(transformation_matrix_);
            for (auto renderable : renderables_)
                renderable->submit(renderer);

            renderer.get_stack().pop();
        }

        inline void add(IRenderable2D* renderable) { renderables_.push_back(renderable); }

      private:
        std::vector<IRenderable2D*> renderables_;
        glm::mat4 transformation_matrix_;
    };
} // namespace ce
