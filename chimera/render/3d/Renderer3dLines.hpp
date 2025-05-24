#pragma once
#include "IRenderer3d.hpp"
#include "chimera/core/gl/DrawLine.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/space/Frustum.hpp"

namespace ce {

    class Renderer3dLines : public IRenderer3d {
      private:
        DrawLine drawLine;
        Frustum frustum;

      public:
        Renderer3dLines() noexcept = default;
        virtual ~Renderer3dLines() noexcept { destroy(); };

        virtual void begin(Camera* camera, IViewProjection* vpo, Octree* octree) override;
        virtual void submit(const RenderCommand& command, IRenderable3d* renderable, const uint32_t& count) override;
        virtual void end() override;
        virtual void flush() override;

        bool valid() noexcept { return drawLine.valid(); }
        void destroy() noexcept { drawLine.destroy(); };
        void create(std::shared_ptr<Shader> shader, const uint32_t& sizeBuffer) noexcept {
            drawLine.create(shader, sizeBuffer);
        };
    };

} // namespace ce
