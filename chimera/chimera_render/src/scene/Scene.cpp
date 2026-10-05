#include "chimera_render/scene/Scene.hpp"
#include "chimera_core/bullet/Solid.hpp"
#include "chimera_core/gl/AssetManager.hpp"
#include "chimera_core/gl/RenderCommand.hpp"
#include "chimera_core/visible/CameraControllerFPS.hpp"
#include "chimera_core/visible/CameraControllerOrbit.hpp"
#include "chimera_ecs/CameraComponent.hpp"
#include "chimera_ecs/EmitterComponent.hpp"
#include "chimera_ecs/LightComponent.hpp"
#include "chimera_ecs/MaterialComponent.hpp"
#include "chimera_ecs/MeshComponent.hpp"
#include "chimera_ecs/Renderable3dComponent.hpp"
#include "chimera_ecs/ShaderComponent.hpp"
#include "chimera_ecs/TransComponent.hpp"
#include "chimera_render/2d/Tile.hpp"
#include "chimera_render/3d/RenderableArray.hpp"
#include "chimera_render/3d/RenderableBsp.hpp"
#include "chimera_render/3d/RenderableMesh.hpp"
#include "chimera_render/3d/RenderableParticles.hpp"
#include "chimera_render/3d/Renderer3d.hpp"
#include <memory>

namespace ce {

    Scene::Scene(std::shared_ptr<entt::registry> registry) : origem_(nullptr), verbose_(0), registry_(registry) {
        this->canvas_ = std::dynamic_pointer_cast<ce::CanvasGL>(registry_->ctx().get<std::shared_ptr<ce::ICanva>>());
        if (this->canvas_ == nullptr) {
            throw std::runtime_error("Canva not found in CTX");
        }
    }

    Scene::~Scene() {
        if (shadow_data_.shadowBuffer) {
            shadow_data_.shadowBuffer.reset();
        }
    }

    std::shared_ptr<RenderBuffer> Scene::initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width,
                                                const uint32_t& height) {
        if (!e_render_bufer_spec_) {
            throw std::string("RenderBuffer nao encontrado");
        }

        // Define o framebuffer de desenho
        FrameBufferSpecification& fbSpec = e_render_bufer_spec_.getComponent<FrameBufferSpecification>(registry_.get());
        fbSpec.width = width;
        fbSpec.height = height;
        auto& sc = e_render_bufer_spec_.getComponent<ShaderComponent>(registry_.get());
        return make_shared<RenderBuffer>(initW, initH, std::make_shared<FrameBuffer>(fbSpec), sc.shader);
    }

    void Scene::createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height) {

        for (auto& rb : v_rb_) {
            rb.reset();
        }

        v_rb_.clear();
        uint32_t halfHidth = width / 2;
        if (size == 2) {
            v_rb_.push_back(initRB(0, 0, halfHidth, height));         // left
            v_rb_.push_back(initRB(halfHidth, 0, halfHidth, height)); // right
        } else {
            v_rb_.push_back(initRB(0, 0, width, height)); // full only
        }
    }

    void Scene::on_deatach() {
        // vpo = nullptr;
        // phyCrt = nullptr;
    }

    void Scene::createOctree(const AABB& aabb) {

        if (octree_ != nullptr) {
            octree_.reset();
        }

        octree_ = std::make_shared<Octree>(aabb, 27, true); // 18
    }

    void Scene::on_attach() {
        // Pega o ViewProjection do ECS antes da camera por caussa do vpo
        vpo_ = registry_->ctx().get<std::shared_ptr<ViewProjection>>();
        auto* ph = registry_->ctx().find<std::shared_ptr<PhysicsControl>>(); // FIXME: ver se nao existir o que retorna

        if (ph != nullptr) {
            phy_crt_.reset(ph->get());
        } else {
            phy_crt_ = nullptr;
        }

        // Totalizadores de area
        glm::vec3 tot_min;
        glm::vec3 tot_max;
        int tot_mesh = 0;

        // lista as tags nas entidades registradas
        for (auto entityID : registry_.get()->view<entt::entity>()) {
            Entity entity(entityID);
            auto& tc = entity.getComponent<TagInfo>(registry_.get());
            SDL_Log("Tag: %s Id: %s", tc.name.c_str(), tc.id.c_str());

            if (tc.name == "TileText") {
                CameraComponent& cCam = entity.getComponent<CameraComponent>(registry_.get());
                auto& sc = entity.getComponent<ShaderComponent>(registry_.get());
                // TileComponent& tc = entity.addComponent<TileComponent>();

                // TODO: passar tile camera para smart
                layers_.push_state(std::make_shared<Tile>("TileText", &batch_render2_d_, sc.shader, cCam.camera));
            }

            // Se for um mesh inicializar componente
            if (entity.hasComponent<MeshComponent>(registry_.get())) {
                MeshComponent& mesh = entity.getComponent<MeshComponent>(registry_.get());

                // Inicializa Materiais
                if (entity.hasComponent<MaterialComponent>(registry_.get())) {
                    MaterialComponent& material = entity.getComponent<MaterialComponent>(registry_.get());
                    if (!material.material->is_valid()) {
                        material.material->init();
                    }
                } else {
                    MaterialComponent& material = entity.addComponent<MaterialComponent>(registry_.get());
                    material.material = std::make_shared<Material>();
                    material.material->set_default_effect();
                    material.material->init();
                }

                // Cria componentes renderizaveis
                Renderable3dComponent& rc = entity.addComponent<Renderable3dComponent>(registry_.get());
                if (mesh.type == MeshType::SIMPLE) {
                    rc.renderable = new RenderableMesh(mesh.mesh);
                } else if (mesh.type == MeshType::ARRAY) {
                    rc.renderable = new RenderableArray(mesh.vTrisIndex, mesh.mesh);
                } else if (mesh.type == MeshType::BSTREE) {
                    rc.renderable = new RenderableBsp(*mesh.mesh);
                }

                auto [min, max, size] = vertexBoundaries(mesh.mesh->vertex);

                if (entity.hasComponent<TransComponent>(registry_.get())) {
                    // Ajuste de fisica se existir
                    TransComponent& tc = entity.getComponent<TransComponent>(registry_.get());
                    if (tc.solid) {
                        // Cria rigidBody iniciaza transformacao e inicializa shape se ele nao existir
                        Solid* solid = (Solid*)tc.trans;
                        // TODO: Era half size ??
                        solid->init(size / 2.0F);
                    }
                }

                // Totalizadores de ambiente
                if (tot_mesh > 0) {
                    tot_min = glm::min(tot_min, min);
                    tot_max = glm::max(tot_max, max);
                } else {
                    tot_min = min;
                    tot_max = max;
                }
                tot_mesh++;
            }

            // Se existir particulas
            if (entity.hasComponent<EmitterComponent>(registry_.get())) {
                EmitterComponent& ec = entity.getComponent<EmitterComponent>(registry_.get());
                if (!entity.hasComponent<RenderableParticlesComponent>(registry_.get())) {
                    RenderableParticlesComponent& particleSys =
                        entity.addComponent<RenderableParticlesComponent>(registry_.get());
                    particleSys.enable = true;
                    RenderableParticles* p = new RenderableParticles();
                    std::shared_ptr<ParticleContainer> pc = ec.emitter->get_container(0); // FIXME: melhorar!!!!
                    p->setParticleContainer(pc);
                    p->create();
                    particleSys.renderable = p;
                    emitters_.push_back(ec.emitter);
                }
            }

            if (entity.hasComponent<FrameBufferSpecification>(registry_.get())) {
                FrameBufferSpecification& fbSpec = entity.getComponent<FrameBufferSpecification>(registry_.get());
                if (tc.name == "shadow01") { // init shadow data

                    auto& sc = entity.getComponent<ShaderComponent>(registry_.get());
                    CameraComponent& cc = entity.getComponent<CameraComponent>(registry_.get());
                    cc.camera->set_viewport_size(fbSpec.width, fbSpec.height);
                    shadow_data_.shader = sc.shader; // entity.getComponent<Shader>();
                    shadow_data_.lightProjection = cc.camera->get_projection();
                    shadow_data_.shadowBuffer = std::make_shared<FrameBuffer>(fbSpec);

                } else if (tc.name == "RenderBufferMaster") {

                    e_render_bufer_spec_ = entity;
                }
            }
        }

        this->onViewportResize(canvas_->width(), canvas_->height());

        { // Registra Camera controllers ViewProjection deve ser localizado acima
            auto view1 = registry_.get()->view<CameraComponent>();
            for (auto entity : view1) {
                Entity e(entity);

                auto& cc = e.getComponent<CameraComponent>(registry_.get());
                if (cc.camKind == CamKind::FPS) {
                    layers_.push_state(std::make_shared<CameraControllerFPS>(registry_, e));
                } else if (cc.camKind == CamKind::ORBIT) {
                    // CameraControllerOrbit* ccOrb = new CameraControllerOrbit(e);
                    layers_.push_state(std::make_shared<CameraControllerOrbit>(registry_, e));
                } else if (cc.camKind == CamKind::STATIC) {
                    // e.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
                }
            }
        }

        origem_ = new Transform(); // FIXME: coisa feia!!!!
        scene_aabb_.setBoundary(tot_min, tot_max);
    }

    void Scene::on_update(const double& ts) {

        if (phy_crt_) {
            phy_crt_->step_sim(ts);
            phy_crt_->check_collisions();
        }

        for (auto* emissor : emitters_) {
            emissor->recycle_life(ts);
        }

        for (auto it = layers_.begin(); it != layers_.end(); it++)
            (*it)->on_update(ts);

        createOctree(scene_aabb_);
    }

    void Scene::onViewportResize(const uint32_t& width, const uint32_t& height) {

        createRenderBuffer(vpo_->get_size(), width, height);

        auto view = registry_.get()->view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.fixedAspectRatio) {

                for (auto renderBuffer : v_rb_) { // altera a matrix de projecao apenas na troca de resolucao
                    cameraComponent.camera->set_viewport_size(renderBuffer->width(), renderBuffer->height());
                    if (cameraComponent.primary) {
                        active_cam_ = cameraComponent.camera;
                    }
                }
            }
        }
    }

    void Scene::on_event(const SDL_Event& event) {

        bool gotcha{true};

        switch (event.type) {
            case SDL_EVENT_WINDOW_RESIZED: {
                onViewportResize(event.window.data1, event.window.data2);
            } break;
            case SDL_EVENT_KEY_DOWN: { // TODO: removar daqui para update!
                switch (event.key.key) {
                    case SDLK_F9: {
                        verbose_++;
                        if (verbose_ > 2)
                            verbose_ = 0;
                    } break;
                    default:
                        gotcha = false;
                        break;
                }
            } break;
            default:
                gotcha = false;
                break;
        }

        for (auto layer : layers_) {
            layer->on_event(event);
        }
    }

    void Scene::renderShadow(IRenderer3d& renderer) {

        renderer.begin(active_cam_, vpo_, nullptr);
        {
            auto lightViewEnt = registry_.get()->view<LightComponent>();
            for (auto entity : lightViewEnt) {
                auto& lc = lightViewEnt.get<LightComponent>(entity);
                auto& tc = registry_.get()->get<TransComponent>(entity); // Lento
                if (lc.global) {
                    // FIXME: usar o direcionm depois no segundo parametro
                    glm::mat4 lightView =
                        glm::lookAt(tc.trans->get_position(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
                    shadow_data_.lightSpaceMatrix = shadow_data_.lightProjection * lightView;
                }
            }

            auto group = registry_.get()->group<TransComponent, Renderable3dComponent>();
            for (auto entity : group) {
                auto [tc, rc] = group.get<TransComponent, Renderable3dComponent>(entity);

                RenderCommand command;
                command.transform = tc.trans->translate_src(origem_->get_position());
                command.shader = shadow_data_.shader;
                command.uniforms["model"] = Uniform(command.transform);
                command.uniforms["lightSpaceMatrix"] = Uniform(shadow_data_.lightSpaceMatrix);
                rc.renderable->submit(command, renderer);
            }
        }

        renderer.end();
        shadow_data_.shadowBuffer->bind(); // we're not using the stencil buffer now

        renderer.flush();
        shadow_data_.shadowBuffer->unbind();
    }

    void Scene::execEmitterPass(IRenderer3d& renderer) {
        auto view = registry_.get()->view<RenderableParticlesComponent>();
        for (auto entity : view) {
            RenderableParticlesComponent& rc = view.get<RenderableParticlesComponent>(entity);
            Renderable3D* renderable = rc.renderable;

            Entity e(entity);
            TransComponent& tc = e.getComponent<TransComponent>(registry_.get()); // FIXME: group this!!!
            auto& sc = e.getComponent<ShaderComponent>(registry_.get());
            MaterialComponent& mc = e.getComponent<MaterialComponent>(registry_.get());

            RenderCommand command;
            command.transform = tc.trans->translate_src(origem_->get_position());
            command.shader = sc.shader;
            mc.material->bind_material_information(command.uniforms, command.vTex);

            const glm::mat4& view = vpo_->get_sel().view;
            command.uniforms["projection"] = Uniform(renderer.getCamera()->get_projection());
            command.uniforms["view"] = Uniform(view);
            command.uniforms["CameraRight_worldspace"] = Uniform(glm::vec3(view[0][0], view[1][0], view[2][0]));
            command.uniforms["CameraUp_worldspace"] = Uniform(glm::vec3(view[0][1], view[1][1], view[2][1]));
            command.uniforms["model"] = Uniform(command.transform);
            renderable->submit(command, renderer);
        }
    }

    void Scene::execRenderPass(IRenderer3d& renderer) {
        // ref:
        // https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system/465d90e0f5961adc460cd9d1e9358370987fbcd3#views-and-groups
        auto group = registry_.get()->view<ShaderComponent, MaterialComponent, TransComponent, Renderable3dComponent>();
        for (auto entity : group) {
            auto [sc, mc, tc, rc] =
                group.get<ShaderComponent, MaterialComponent, TransComponent, Renderable3dComponent>(entity);

            RenderCommand command;
            command.transform = tc.trans->translate_src(origem_->get_position());
            command.shader = sc.shader;
            mc.material->bind_material_information(command.uniforms, command.vTex);
            command.uniforms["model"] = Uniform(command.transform);
            rc.renderable->submit(command, renderer);
        }
    }

    void Scene::on_render() {
        Renderer3d renderer(verbose_ > 0);

        if (verbose_ > 0) {
            const glm::vec3& pos = active_cam_->get_position();
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
        }

        // render a shadows in framebuffer
        if (shadow_data_.shadowBuffer)
            renderShadow(renderer);

        uint8_t count = 0;
        for (auto renderBuffer : v_rb_) {

            vpo_->set_index(count);
            count++;

            // data load used by all
            renderer.uboQueue().insert(std::make_pair("projection", Uniform(active_cam_->get_projection())));
            renderer.uboQueue().insert(std::make_pair("view", Uniform(vpo_->get_sel().view)));

            // data load shadows props to renderer in shade of models!!!!
            if (shadow_data_.shadowBuffer) {
                renderer.uboQueue().insert(std::make_pair("viewPos", Uniform(active_cam_->get_position())));
                renderer.uboQueue().insert(std::make_pair("shadows", Uniform(1)));
                renderer.uboQueue().insert(std::make_pair("shadowMap", Uniform(1)));
                renderer.uboQueue().insert(std::make_pair("lightSpaceMatrix", Uniform(shadow_data_.lightSpaceMatrix)));
                renderer.texQueue().push_back(shadow_data_.shadowBuffer->get_depth_attachemnt());
            }

            // data load lights
            auto lightView = registry_.get()->view<LightComponent>();
            for (auto entity : lightView) {
                auto& lc = lightView.get<LightComponent>(entity);
                auto& tc = registry_.get()->get<TransComponent>(entity); // lightView.get<LightComponent>(entity);
                if (lc.global) {                                         // biding light prop
                    lc.light->bind_light(renderer.uboQueue(), tc.trans->get_matrix());
                }
            }

            renderBuffer->bind(); // bind renderbuffer to draw we're not using the stencil buffer now

            renderer.begin(active_cam_, vpo_, octree_);
            this->execRenderPass(renderer);
            renderer.end();
            renderer.flush();

            if (emitters_.size() > 0) {
                // inicializa state machine do opengl
                BinaryStateEnable depth(GL_DEPTH_TEST, GL_TRUE);
                BinaryStateEnable blender(GL_BLEND, GL_TRUE);
                DepthFuncSetter depthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                renderer.begin(active_cam_, vpo_, nullptr);
                this->execEmitterPass(renderer);
                renderer.end();
                renderer.flush();
            }

            if (verbose_ > 0) {      // RENDER DEBUG
                if (verbose_ == 1) { // DEBUG OCTREE

                    if (!dl_.valid()) {
                        std::unordered_map<GLenum, std::string> shadeData;
                        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
                        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";

                        auto assets = this->registry_->ctx().get<std::shared_ptr<AssetManager>>();

                        dl_.create(assets->load_shader("DrawLine", shadeData).handle(), 40000);
                    }

                    if (octree_ != nullptr) {

                        std::vector<AABB> list;
                        octree_->getBondaryList(list, false);

                        for (auto& aabb : list) {
                            dl_.add_aabb(aabb, glm::vec3(1.0, 1.0, 1.0));
                        }

                        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Octree Size: %ld", list.size());

                        MapUniform muni;
                        muni["projection"] = Uniform(active_cam_->get_projection());
                        muni["view"] = Uniform(vpo_->get_sel().view);
                        dl_.render(muni);
                    }

                } else if (verbose_ == 2) { // DEBUG AABB

                    if (!render_lines_.valid()) {
                        std::unordered_map<GLenum, std::string> shadeData;
                        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
                        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";

                        auto assets = this->registry_->ctx().get<std::shared_ptr<AssetManager>>();

                        render_lines_.create(assets->load_shader("DrawLine", shadeData).handle(), 10000);
                    }

                    render_lines_.begin(active_cam_, vpo_, nullptr);

                    render_lines_.uboQueue().insert(
                        std::make_pair("projection", Uniform(active_cam_->get_projection())));
                    render_lines_.uboQueue().insert(std::make_pair("view", Uniform(vpo_->get_sel().view)));

                    auto group = registry_.get()->group<TransComponent, Renderable3dComponent>();
                    for (auto entity : group) {
                        auto [tc, rc] = group.get<TransComponent, Renderable3dComponent>(entity);

                        RenderCommand command;
                        command.transform = tc.trans->translate_src(origem_->get_position());
                        rc.renderable->submit(command, render_lines_);
                    }

                    auto view = registry_.get()->view<RenderableParticlesComponent>();
                    for (auto entity : view) {
                        RenderableParticlesComponent& rc = view.get<RenderableParticlesComponent>(entity);
                        Renderable3D* renderable = rc.renderable;

                        Entity e(entity);
                        TransComponent& tc = e.getComponent<TransComponent>(registry_.get()); // FIXME: group this!!!

                        RenderCommand command;
                        command.transform = tc.trans->translate_src(origem_->get_position());

                        renderable->submit(command, render_lines_);
                    }

                    render_lines_.end();
                    render_lines_.flush();
                }
            }

            for (auto it = layers_.begin(); it != layers_.end(); it++)
                (*it)->on_render();

            {
                // TODO: captura do entity no framebuffer da tela
                // get val from color buffer (must be inside framebuffer renderer
                // glm::ivec2 pos = Mouse::getMove();
                // pos.y = viewportHeight - pos.y;
                // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "mouse(X: %d / Y: %d): %d", pos.x, pos.y, val);
            }

            renderBuffer->unbind();
            renderBuffer->render();
        }
    }
} // namespace ce
