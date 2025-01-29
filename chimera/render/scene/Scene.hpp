#pragma once
#include "bullet/interfaces.hpp"
#include "core/IStateMachine.hpp"
#include "core/Registry.hpp"
#include "core/StateStack.hpp"
#include "core/interfaces.hpp"
#include "coreGL/ParticleEmitter.hpp"
#include "coreGL/RenderBuffer.hpp"
#include "render/2d/BatchRender2D.hpp"
#include "render/3d/IRenderable3d.hpp"
#include "render/3d/Renderer3dLines.hpp"
#include "space/ICamera.hpp"
#include "space/ITrans.hpp"
#include "space/Octree.hpp"

namespace ce {

struct ShadowData {
    ShadowData() = default;
    std::shared_ptr<Shader> shader;
    FrameBuffer* shadowBuffer = nullptr;
    glm::mat4 lightSpaceMatrix = glm::mat4(1.0f), lightProjection = glm::mat4(1.0f);
};

class Entity;
class Scene : public IStateMachine {
  private:
    // std::shared_ptr<Registry> registry;
    // std::shared_ptr<ServiceLocator> serviceLoc;
    std::shared_ptr<IViewProjection> vpo;
    std::shared_ptr<IPhysicsControl> phyCrt;

    StateStack layers;

    ITrans* origem;
    Camera* activeCam;

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

  public:
    Scene();
    virtual ~Scene();
    void setOrigem(ITrans* o) { origem = o; }
    StateStack& getLayes() { return this->layers; }
    // Herdados
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual const std::string getName() override { return "Scene"; }

  private:
    void onViewportResize(const uint32_t& width, const uint32_t& height);
    void createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height);
    void execRenderPass(IRenderer3d& renderer);
    void execEmitterPass(IRenderer3d& renderer);
    void renderShadow(IRenderer3d& renderer);
    RenderBuffer* initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width, const uint32_t& height);
    void createOctree(const AABB& aabb);
};
} // namespace ce
