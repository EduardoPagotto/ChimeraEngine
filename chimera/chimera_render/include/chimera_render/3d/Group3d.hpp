#pragma once
#include "IRenderer3d.hpp"
#include "chimera_base/aux/TransformationStack.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace ce {

    class Group3d : public IRenderable3d {
      public:
        Group3d(const glm::mat4& transform) : transformation_matrix_(transform) {}
        virtual ~Group3d() {}
        virtual void submit(RenderCommand& command, IRenderer3d& renderer) override {
            renderer.getStack().push(transformation_matrix_);
            for (auto renderable : renderables)
                renderable->submit(command, renderer);
            renderer.getStack().pop();
        }

        inline void add(IRenderable3d* renderable) { renderables.push_back(renderable); }

      private:
        std::vector<IRenderable3d*> renderables_;
        glm::mat4 transformation_matrix_;
    };
} // namespace ce
