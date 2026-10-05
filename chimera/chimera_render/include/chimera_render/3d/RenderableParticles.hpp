#pragma once
#include "IRenderer3d.hpp"
#include "chimera_core/gl/ParticleEmitter.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_core/gl/buffer/VertexArray.hpp"
#include "chimera_space/AABB.hpp"

namespace ce {

    class RenderableParticles : public Renderable3D {
      public:
        RenderableParticles() = default;

        virtual ~RenderableParticles();

        const uint32_t get_size() const override { return pc_->particlesCount; }

        std::shared_ptr<IndexBuffer> get_ibo() const override { return nullptr; }

        const AABB& get_aabb() const override { return pc_->aabb; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

        void draw(const bool& log_data) override;

        void create();

        void destroy();

        void set_particle_container(std::shared_ptr<ParticleContainer> pc) { this->pc_ = pc; }

      private:
        std::shared_ptr<VertexBuffer> vbo_vex_;
        std::shared_ptr<VertexBuffer> vbo_pos_;
        std::shared_ptr<VertexBuffer> vbo_cor_;
        std::shared_ptr<ParticleContainer> pc_;
    };
} // namespace ce
