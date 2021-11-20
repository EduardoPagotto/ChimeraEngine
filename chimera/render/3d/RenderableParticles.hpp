#pragma once
#include "IRenderable3d.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/render/ParticleEmitter.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"

namespace Chimera {
class RenderableParticles : public IRenderable3d {
  public:
    RenderableParticles() = default;
    virtual ~RenderableParticles();

    virtual void debugDados() override {}                   // TODO: Implementar
    virtual uint32_t getSize() const override { return 0; } // TODO: Implementar
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual Entity getEntity() const override { return entity; }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    void draw();
    void create();
    void destroy();
    void setEntity(Entity entity) { this->entity = entity; }
    void setParticleContainer(ParticleContainer* pc) { this->pc = pc; }

  private:
    AABB aabb;
    VertexArray* vao = nullptr;
    VertexBuffer* vboVex = nullptr; // GLuint vboVertex;
    VertexBuffer* vboPos = nullptr; // GLuint vboPosition;
    VertexBuffer* vboCor = nullptr; // GLuint vboColor;
    Entity entity;
    ParticleContainer* pc;
};
} // namespace Chimera