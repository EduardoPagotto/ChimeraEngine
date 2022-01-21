#pragma once
#include "IRenderable3d.hpp"
#include "chimera/render/ParticleEmitter.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"

namespace Chimera {
class RenderableParticles : public IRenderable3d {
  public:
    RenderableParticles() = default;
    virtual ~RenderableParticles();

    virtual uint32_t getSize() const override { return pc->particlesCount; }
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return pc->aabb; }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    virtual void draw(const bool& logData) override;
    void create();
    void destroy();
    void setParticleContainer(ParticleContainer* pc) { this->pc = pc; }

  private:
    VertexArray* vao = nullptr;
    VertexBuffer* vboVex = nullptr;
    VertexBuffer* vboPos = nullptr;
    VertexBuffer* vboCor = nullptr;
    ParticleContainer* pc;
};
} // namespace Chimera