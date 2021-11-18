#pragma once
#include "IRenderable3d.hpp"
#include "chimera/render/ParticleEmitter.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/partition/AABB.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {
class RenderableParticleEmitter : public IRenderable3d {
  public:
    RenderableParticleEmitter() = default;
    virtual ~RenderableParticleEmitter();

    virtual void debugDados() override {}                   // TODO: Implementar
    virtual uint32_t getSize() const override { return 0; } // TODO: Implementar
    virtual VertexArray* getVao() const override { return vao; }
    virtual IndexBuffer* getIBO() const override { return nullptr; }
    virtual const AABB& getAABB() const override { return aabb; }
    virtual Entity getEntity() const override { return entity; }
    virtual void submit(ICamera* camera, RenderCommand& command, IRenderer3d* renderer) override;
    void draw();
    void create(uint32_t max);
    void destroy();
    void setEntity(Entity entity) { this->entity = entity; }
    ParticleContainer* getParticleContainer() { return &pc; }

  private:
    AABB aabb;
    VertexArray* vao = nullptr;
    VertexBuffer* vboVex = nullptr; // GLuint vboVertex;
    VertexBuffer* vboPos = nullptr; // GLuint vboPosition;
    VertexBuffer* vboCor = nullptr; // GLuint vboColor;
    Entity entity;
    ParticleContainer pc;
};
} // namespace Chimera