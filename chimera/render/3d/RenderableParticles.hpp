#pragma once
#include "chimera/core/gl/ParticleEmitter.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/space/AABB.hpp"
#include "chimera/render/3d/Renderable3D.hpp"

namespace ce {
class RenderableParticles : public Renderable3D {
  public:
    RenderableParticles() = default;
    virtual ~RenderableParticles();

    virtual const uint32_t getSize() const override { return pc->particlesCount; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return pc->aabb; }
    virtual void submit(RenderCommand& command, IRenderer3d& renderer) override;
    virtual void draw(const bool& logData) override;
    void create();
    void destroy();
    void setParticleContainer(ParticleContainer* pc) { this->pc = pc; }

  private:
    VertexBuffer* vboVex = nullptr;
    VertexBuffer* vboPos = nullptr;
    VertexBuffer* vboCor = nullptr;
    ParticleContainer* pc;
};
} // namespace ce