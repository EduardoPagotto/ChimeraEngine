#pragma once
#include "ShadowPass.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/buffer/RenderBuffer.hpp"
#include "chimera/render/bullet/PhysicsControl.hpp"

namespace Chimera {

class Entity;
class Scene : public IStateMachine {
  public:
    Scene();
    virtual ~Scene();
    void onViewportResize(uint32_t width, uint32_t height);
    void logToggle() { logRender = !logRender; }
    Registry& getRegistry() { return registry; }
    // Herdados
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const override { return "Scene"; }
    void pushEmitters(IEmitter* e) { emitters.push_back(e); }
    void setOrigem(ITrans* o) { origem = o; }
    void setShadowPass(ShadowPass* shadowPass) { this->shadowPass = shadowPass; }

  private:
    RenderBuffer* initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width, const uint32_t& height);
    void createRenderBuffer(EyeView* eyeView);
    void execRenderPass(ICamera* camera, IRenderer3d& renderer);
    void execShadowPass(ICamera* camera, IRenderer3d& renderer);
    void execEmitterPass(ICamera* camera, IRenderer3d& renderer);
    uint32_t viewportWidth, viewportHeight;
    Registry registry;
    ICamera* camera;
    std::vector<RenderBuffer*> vRB;
    ShadowPass* shadowPass;
    PhysicsControl* physicsControl;
    Renderer3d renderBatch;
    std::vector<IEmitter*> emitters;
    ITrans* origem;
    bool logRender;
};
} // namespace Chimera
