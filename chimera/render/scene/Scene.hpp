#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/3d/RendererParticleEmitter.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/ParticleEmitter.hpp"
#include "chimera/render/buffer/RenderBuffer.hpp"
#include "chimera/render/bullet/PhysicsControl.hpp"
#include <entt/entt.hpp>

namespace Chimera {

struct ShadowPass {
    Shader shader;
    FrameBuffer* shadowBuffer;
    glm::mat4 lightSpaceMatrix, lightProjection;
};

class Entity;
class Scene : public IStateMachine {
    friend class Entity;

  public:
    Scene();
    virtual ~Scene();
    Entity createEntity(const std::string& name = std::string());
    void destroyEntity(Entity entity);
    void onViewportResize(uint32_t width, uint32_t height);
    IRenderer3d* getRender() { return &renderBatch; }
    entt::registry& getRegistry() { return eRegistry; }
    // Herdados
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const { return "Scene"; }

  private:
    void createRenderBuffer();
    void execRenderPass(ICamera* camera, IRenderer3d& renderer);
    void execShadowPass(ICamera* camera, IRenderer3d& renderer);
    void render(IRenderer3d& renderer);
    Renderer3d renderBatch;
    uint32_t viewportWidth, viewportHeight;
    entt::registry eRegistry;
    ICamera* camera;
    RenderBuffer* renderBuffer;
    ShadowPass shadowPass;
    PhysicsControl* physicsControl;
    RendererParticleEmitter renderParticleEmitter;
    IEmitter* emissor;
};
} // namespace Chimera
