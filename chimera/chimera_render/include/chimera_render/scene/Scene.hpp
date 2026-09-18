#pragma once
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/StateStack.hpp"
#include "chimera_base/aux/ICamera.hpp"
#include "chimera_base/aux/Transform.hpp"
#include "chimera_core/bullet/interfaces.hpp"
#include "chimera_core/gl/CanvasGL.hpp"
#include "chimera_core/gl/ParticleEmitter.hpp"
#include "chimera_core/gl/buffer/RenderBuffer.hpp"
#include "chimera_ecs/Entity.hpp"
#include "chimera_render/2d/BatchRender2D.hpp"
#include "chimera_render/3d/Renderer3dLines.hpp"
#include "chimera_space/Octree.hpp"

namespace ce {

    struct ShadowData {
        ShadowData() = default;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<FrameBuffer> shadowBuffer;
        glm::mat4 lightSpaceMatrix = glm::mat4(1.0f), lightProjection = glm::mat4(1.0f);
    };

    class Entity;
    class Scene : public IStateMachine {
      public:
        Scene(std::shared_ptr<entt::registry> registry);
        virtual ~Scene();
        void setOrigem(ITrans* o) { origem = o; }
        StateStack& getLayes() { return this->layers; }
        // Herdados
        virtual void onAttach() override;
        virtual void onDeatach() override;
        virtual void onRender() override;
        virtual void onUpdate(const double& ts) override;
        virtual void onEvent(const SDL_Event& event) override;
        std::string getName() const override { return "SCENE"; }

      private:
        void onViewportResize(const uint32_t& width, const uint32_t& height);
        void createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height);
        void execRenderPass(IRenderer3d& renderer);
        void execEmitterPass(IRenderer3d& renderer);
        void renderShadow(IRenderer3d& renderer);
        std::shared_ptr<RenderBuffer> initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width,
                                             const uint32_t& height);
        void createOctree(const AABB& aabb);

        std::shared_ptr<ViewProjection> vpo;
        std::shared_ptr<IPhysicsControl> phyCrt;
        std::shared_ptr<Camera> activeCam;
        std::shared_ptr<Octree> octree;
        std::shared_ptr<ce::CanvasGL> canvas;

        StateStack layers;
        ITrans* origem;

        ShadowData shadowData;
        uint8_t verbose;

        std::vector<std::shared_ptr<RenderBuffer>> vRB;
        std::vector<IEmitter*> emitters;

        Entity eRenderBuferSpec;
        BatchRender2D batchRender2D;

        AABB sceneAABB;
        Renderer3dLines renderLines;

        DrawLine dl;

        std::shared_ptr<entt::registry> registry;
    };
} // namespace ce
