#pragma once
#include "IRenderer3d.hpp"
#include "chimera_core/gl/DrawLine.hpp"
#include "chimera_space/Frustum.hpp"

namespace ce {

    class Renderer3dLines : public IRenderer3d {
      public:
        Renderer3dLines() noexcept = default;

        virtual ~Renderer3dLines() noexcept { destroy(); };

        virtual void begin(std::shared_ptr<Camera> camera, std::shared_ptr<ViewProjection> vpo,
                           std::shared_ptr<Octree> octree) override;

        virtual void submit(const RenderCommand& command, Renderable3D* renderable, const uint32_t& count) override;

        virtual void end() override;

        virtual void flush() override;

        bool valid() noexcept { return draw_line_.valid(); }

        void destroy() noexcept { draw_line_.destroy(); };

        void create(std::shared_ptr<Shader> shader, const uint32_t& size_buffer) noexcept {
            draw_line_.create(shader, size_buffer);
        };

      private:
        DrawLine draw_line_;
        Frustum frustum_;
    };

} // namespace ce
