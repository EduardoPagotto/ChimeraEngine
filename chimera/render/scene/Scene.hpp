#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/StateStack.hpp"
#include "chimera/core/buffer/RenderBuffer.hpp"
#include "chimera/core/device/interfaces.hpp"
#include "chimera/core/space/Octree.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/ITrans.hpp"
#include "chimera/core/visible/ParticleEmitter.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include "chimera/render/3d/Renderer3dLines.hpp"

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
    Scene();
    virtual ~Scene();
    void setOrigem(ITrans* o) { origem = o; }
    ICanva* getCanvas();
    StateStack& getLayes() { return this->layers; }
    // Herdados
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    void onViewportResize(const uint32_t& width, const uint32_t& height);
    void createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height);
    void execRenderPass(IRenderer3d& renderer);
    void execEmitterPass(IRenderer3d& renderer);
    void renderShadow(IRenderer3d& renderer);
    RenderBuffer* initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width, const uint32_t& height);

    void createOctree(const AABB& aabb);

    StateStack layers;
    Registry* registry;
    ITrans* origem;
    Camera* activeCam;
    IViewProjection* vpo;
    ShadowData shadowData;
    uint8_t verbose;
    std::vector<RenderBuffer*> vRB;
    std::vector<IEmitter*> emitters;
    Entity eRenderBuferSpec;
    BatchRender2D batchRender2D;

    Octree* octree;
    AABB sceneAABB;
    Renderer3dLines renderLines;

    DrawLine dl;
};
} // namespace Chimera
