#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/TransformationStack.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace ce {

    class Group3d : public IRenderable3d {

      private:
        std::vector<IRenderable3d*> renderables;
        glm::mat4 transformationMatrix;

      public:
        Group3d(const glm::mat4& transform) : transformationMatrix(transform) {}
        virtual ~Group3d() {}
        virtual void submit(RenderCommand& command, IRenderer3d& renderer) override {
            renderer.getStack().push(transformationMatrix);
            for (auto renderable : renderables)
                renderable->submit(command, renderer);
            renderer.getStack().pop();
        }

        inline void add(IRenderable3d* renderable) { renderables.push_back(renderable); }
    };
} // namespace ce
