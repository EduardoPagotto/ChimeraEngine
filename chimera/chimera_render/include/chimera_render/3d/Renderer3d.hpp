#pragma once
#include "IRenderer3d.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_render/3d/IRenderer3d.hpp"
#include "chimera_space/Frustum.hpp"
#include "chimera_space/Octree.hpp"

namespace ce {

    class Renderer3d : public IRenderer3d {
      public:
        Renderer3d(const bool& logData);

        virtual ~Renderer3d();

        virtual void begin(std::shared_ptr<Camera> camera, std::shared_ptr<ViewProjection> vpo,
                           std::shared_ptr<Octree> octree) override;

        virtual void submit(const RenderCommand& command, Renderable3D* renderable, const uint32_t& count) override;

        virtual void end() override;

        virtual void flush() override;

        virtual inline std::vector<std::shared_ptr<Texture>>& texQueue() { return texture_queue_; }

      private:
        std::queue<uint32_t> q_renderable_indexes_;
        std::vector<RenderCommand> v_render_command_;
        std::vector<Renderable3D*> v_renderable_;
        std::vector<std::shared_ptr<Texture>> texture_queue_;
        std::shared_ptr<Octree> octree_;
        Frustum frustum_;
        bool log_data_;
    };
} // namespace ce
