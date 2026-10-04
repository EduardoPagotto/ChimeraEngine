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
        void setOrigem(ITrans* o) { origem_ = o; }
        StateStack& getLayes() { return this->layers_; }
        // Herdados
        virtual void on_attach() override;
        virtual void on_deatach() override;
        virtual void on_render() override;
        virtual void on_update(const double& ts) override;
        virtual void on_event(const SDL_Event& event) override;
        std::string get_name() const override { return "SCENE"; }

      private:
        void onViewportResize(const uint32_t& width, const uint32_t& height);
        void createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height);
        void execRenderPass(IRenderer3d& renderer);
        void execEmitterPass(IRenderer3d& renderer);
        void renderShadow(IRenderer3d& renderer);
        std::shared_ptr<RenderBuffer> initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width,
                                             const uint32_t& height);
        void createOctree(const AABB& aabb);

        std::shared_ptr<ViewProjection> vpo_;
        std::shared_ptr<IPhysicsControl> phy_crt_;
        std::shared_ptr<Camera> active_cam_;
        std::shared_ptr<Octree> octree_;
        std::shared_ptr<ce::CanvasGL> canvas_;

        StateStack layers_;
        ITrans* origem_;

        ShadowData shadow_data_;
        uint8_t verbose_;

        std::vector<std::shared_ptr<RenderBuffer>> v_rb_;
        std::vector<IEmitter*> emitters_;

        Entity e_render_bufer_spec_;
        BatchRender2D batch_render2_d_;

        AABB scene_aabb_;
        Renderer3dLines render_lines_;

        DrawLine dl_;

        std::shared_ptr<entt::registry> registry_;
    };
} // namespace ce
