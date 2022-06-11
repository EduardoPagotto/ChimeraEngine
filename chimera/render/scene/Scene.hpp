#pragma once
#include "ShadowPass.hpp"
//#include "chimera/core/StateStack.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/buffer/RenderBuffer.hpp"
#include "chimera/core/device/Canvas.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"
#include "chimera/render/3d/Renderer3d.hpp"

namespace Chimera {

class Entity;
class Scene : public IStateMachine {
  public:
    Scene();
    virtual ~Scene();
    Registry& getRegistry() { return registry; }
    void pushEmitters(IEmitter* e) { emitters.push_back(e); }
    void setOrigem(ITrans* o) { origem = o; }
    void setShadowPass(ShadowPass* shadowPass) { this->shadowPass = shadowPass; }
    Canvas* getCanvas();
    // void setStack(StateStack& stack) { this->stack = &stack; }
    // StateStack* getStack() const { return this->stack; }
    // Herdados
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const override { return "Scene"; }

  private:
    void onViewportResize(const uint32_t& width, const uint32_t& height);
    void createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height);
    void execRenderPass(ICamera* camera, IRenderer3d& renderer);
    void execShadowPass(ICamera* camera, IRenderer3d& renderer);
    void execEmitterPass(ICamera* camera, IRenderer3d& renderer);
    RenderBuffer* initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width, const uint32_t& height);

    // StateStack* stack;
    ITrans* origem;
    ICamera* activeCam;
    EyeView* eyeView;
    ShadowPass* shadowPass;
    bool logRender;
    Registry registry;
    Renderer3d renderBatch;
    std::vector<RenderBuffer*> vRB;
    std::vector<IEmitter*> emitters;
};
} // namespace Chimera
