#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/3d/RendererParticles.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/ITrans.hpp"
#include "chimera/render/ParticleEmitter.hpp"
#include "chimera/render/buffer/RenderBuffer.hpp"
#include "chimera/render/bullet/PhysicsControl.hpp"

namespace Chimera {

struct ShadowPass {
    Shader shader;
    FrameBuffer* shadowBuffer;
    glm::mat4 lightSpaceMatrix, lightProjection;
};

class Entity;
class Scene : public IStateMachine {
  public:
    Scene();
    virtual ~Scene();
    void onViewportResize(uint32_t width, uint32_t height);
    IRenderer3d* getRender() { return &renderBatch; }
    Registry& getRegistry() { return registry; }
    // Herdados
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const { return "Scene"; }
    void pushEmitters(IEmitter* e) { emitters.push_back(e); }
    void setOrigem(ITrans* o) { origem = o; }

  private:
    void createRenderBuffer();
    void execRenderPass(ICamera* camera, IRenderer3d& renderer);
    void execShadowPass(ICamera* camera, IRenderer3d& renderer);
    void execEmitterPass(ICamera* camera, IRenderer3d& renderer);
    uint32_t viewportWidth, viewportHeight;
    Registry registry;
    ICamera* camera;
    RenderBuffer* renderBuffer;
    ShadowPass shadowPass;
    PhysicsControl* physicsControl;
    Renderer3d renderBatch;
    RendererParticles renderParticleEmitter;
    std::vector<IEmitter*> emitters;
    ITrans* origem;
};
} // namespace Chimera
