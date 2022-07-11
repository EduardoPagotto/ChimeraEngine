#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/StateStack.hpp"
#include "chimera/core/buffer/RenderBuffer.hpp"
#include "chimera/core/device/Canvas.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/ITrans.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"

namespace Chimera {

struct ShadowData {
    ShadowData() = default;
    Shader shader;
    FrameBuffer* shadowBuffer = nullptr;
    glm::mat4 lightSpaceMatrix = glm::mat4(1.0f), lightProjection = glm::mat4(1.0f);
};

class Entity;
class Scene : public IStateMachine {
  public:
    Scene(Registry& r, StateStack& s);
    virtual ~Scene();
    void setOrigem(ITrans* o) { origem = o; }
    Canvas* getCanvas();
    Registry* getRegistry() { return registry; }
    StateStack* getStack() const { return this->stack; }
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
    void execRenderPass(IRenderer3d& renderer);
    void execEmitterPass(IRenderer3d& renderer);
    void renderShadow(IRenderer3d& renderer);
    RenderBuffer* initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width, const uint32_t& height);

    StateStack* stack;
    Registry* registry;
    ITrans* origem;
    Camera* activeCam;
    EyeView* eyeView;
    ShadowData shadowData;
    bool logRender;
    std::vector<RenderBuffer*> vRB;
    std::vector<IEmitter*> emitters;
    Entity eRenderBuferSpec;
};
} // namespace Chimera
