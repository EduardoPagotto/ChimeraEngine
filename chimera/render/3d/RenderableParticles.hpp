#pragma once
#include "chimera/core/gl/ParticleEmitter.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/space/AABB.hpp"

namespace ce {

    class RenderableParticles : public Renderable3D {

      private:
        std::shared_ptr<VertexBuffer> vboVex;
        std::shared_ptr<VertexBuffer> vboPos;
        std::shared_ptr<VertexBuffer> vboCor;
        std::shared_ptr<ParticleContainer> pc;

      public:
        RenderableParticles() = default;

        virtual ~RenderableParticles();

        const uint32_t getSize() const override { return pc->particlesCount; }

        std::shared_ptr<IndexBuffer> getIBO() const override { return nullptr; }

        const AABB& getAABB() const override { return pc->aabb; }

        void submit(RenderCommand& command, IRenderer3d& renderer) override;

        void draw(const bool& logData) override;

        void create();

        void destroy();

        void setParticleContainer(std::shared_ptr<ParticleContainer> pc) { this->pc = pc; }
    };
} // namespace ce
