#pragma once
#include "chimera/base/ICamera.hpp"
#include "chimera/base/ICanva.hpp"
#include "chimera/base/IStateMachine.hpp"
#include "chimera/base/StateStack.hpp"
#include "chimera/base/Transform.hpp"
#include "chimera/core/bullet/interfaces.hpp"
#include "chimera/core/gl/ParticleEmitter.hpp"
#include "chimera/core/gl/buffer/RenderBuffer.hpp"
#include "chimera/ecs/Entity.hpp"
#include "chimera/render/2d/BatchRender2D.hpp"
#include "chimera/render/3d/Renderer3dLines.hpp"
#include "chimera/space/Octree.hpp"

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
        std::shared_ptr<ViewProjection> vpo;
        std::shared_ptr<IPhysicsControl> phyCrt;
        std::shared_ptr<Camera> activeCam;
        std::shared_ptr<Octree> octree;

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

      private:
        void onViewportResize(const uint32_t& width, const uint32_t& height);
        void createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height);
        void execRenderPass(IRenderer3d& renderer);
        void execEmitterPass(IRenderer3d& renderer);
        void renderShadow(IRenderer3d& renderer);
        std::shared_ptr<RenderBuffer> initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width,
                                             const uint32_t& height);
        void createOctree(const AABB& aabb);
    };
} // namespace ce
