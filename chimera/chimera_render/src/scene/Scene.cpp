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

    std::shared_ptr<RenderBuffer> Scene::init_rb(const uint32_t& init_w, const uint32_t& init_h, const uint32_t& width,
                                                 const uint32_t& height) {
        if (!e_render_bufer_spec_) {
            throw std::string("RenderBuffer nao encontrado");
        }

        // Define o framebuffer de desenho
        FrameBufferSpecification& fb_spec =
            e_render_bufer_spec_.get_component<FrameBufferSpecification>(registry_.get());
        fb_spec.width = width;
        fb_spec.height = height;
        auto& sc = e_render_bufer_spec_.get_component<ShaderComponent>(registry_.get());
        return make_shared<RenderBuffer>(init_w, init_h, std::make_shared<FrameBuffer>(fb_spec), sc.shader);
    }

    void Scene::create_render_buffer(const uint8_t& size, const uint32_t& width, const uint32_t& height) {

        for (auto& rb : v_rb_) {
            rb.reset();
        }

        v_rb_.clear();
        uint32_t half_hidth = width / 2;
        if (size == 2) {
            v_rb_.push_back(init_rb(0, 0, half_hidth, height));          // left
            v_rb_.push_back(init_rb(half_hidth, 0, half_hidth, height)); // right
        } else {
            v_rb_.push_back(init_rb(0, 0, width, height)); // full only
        }
    }

    void Scene::on_deatach() {
        // vpo = nullptr;
        // phyCrt = nullptr;
    }

    void Scene::create_octree(const AABB& aabb) {

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
        for (auto entity_id : registry_.get()->view<entt::entity>()) {
            Entity entity(entity_id);
            auto& tc = entity.get_component<TagInfo>(registry_.get());
            SDL_Log("Tag: %s Id: %s", tc.name.c_str(), tc.id.c_str());

            if (tc.name == "TileText") {
                CameraComponent& c_cam = entity.get_component<CameraComponent>(registry_.get());
                auto& sc = entity.get_component<ShaderComponent>(registry_.get());
                // TileComponent& tc = entity.addComponent<TileComponent>();

                // TODO: passar tile camera para smart
                layers_.push_state(std::make_shared<Tile>("TileText", &batch_render2_d_, sc.shader, c_cam.camera));
            }

            // Se for um mesh inicializar componente
            if (entity.has_component<MeshComponent>(registry_.get())) {
                MeshComponent& mesh = entity.get_component<MeshComponent>(registry_.get());

                // Inicializa Materiais
                if (entity.has_component<MaterialComponent>(registry_.get())) {
                    MaterialComponent& material = entity.get_component<MaterialComponent>(registry_.get());
                    if (!material.material->is_valid()) {
                        material.material->init();
                    }
                } else {
                    MaterialComponent& material = entity.add_component<MaterialComponent>(registry_.get());
                    material.material = std::make_shared<Material>();
                    material.material->set_default_effect();
                    material.material->init();
                }

                // Cria componentes renderizaveis
                Renderable3dComponent& rc = entity.add_component<Renderable3dComponent>(registry_.get());
                if (mesh.type == MeshType::SIMPLE) {
                    rc.renderable = new RenderableMesh(mesh.mesh);

                } else if (mesh.type == MeshType::ARRAY) {
                    rc.renderable = new RenderableArray(mesh.vTrisIndex, mesh.mesh);

                } else if (mesh.type == MeshType::BSTREE) {
                    rc.renderable = new RenderableBsp(*mesh.mesh);
                }

                auto [min, max, size] = vertexBoundaries(mesh.mesh->vertex);

                if (entity.has_component<TransComponent>(registry_.get())) {
                    // Ajuste de fisica se existir
                    TransComponent& tc = entity.get_component<TransComponent>(registry_.get());
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
            if (entity.has_component<EmitterComponent>(registry_.get())) {
                EmitterComponent& ec = entity.get_component<EmitterComponent>(registry_.get());
                if (!entity.has_component<RenderableParticlesComponent>(registry_.get())) {
                    RenderableParticlesComponent& particle_sys =
                        entity.add_component<RenderableParticlesComponent>(registry_.get());
                    particle_sys.enable = true;
                    RenderableParticles* p = new RenderableParticles();
                    std::shared_ptr<ParticleContainer> pc = ec.emitter->get_container(0); // FIXME: melhorar!!!!
                    p->set_particle_container(pc);
                    p->create();
                    particle_sys.renderable = p;
                    emitters_.push_back(ec.emitter);
                }
            }

            if (entity.has_component<FrameBufferSpecification>(registry_.get())) {
                FrameBufferSpecification& fb_spec = entity.get_component<FrameBufferSpecification>(registry_.get());
                if (tc.name == "shadow01") { // init shadow data

                    auto& sc = entity.get_component<ShaderComponent>(registry_.get());
                    CameraComponent& cc = entity.get_component<CameraComponent>(registry_.get());
                    cc.camera->set_viewport_size(fb_spec.width, fb_spec.height);
                    shadow_data_.shader = sc.shader; // entity.getComponent<Shader>();
                    shadow_data_.lightProjection = cc.camera->get_projection();
                    shadow_data_.shadowBuffer = std::make_shared<FrameBuffer>(fb_spec);

                } else if (tc.name == "RenderBufferMaster") {

                    e_render_bufer_spec_ = entity;
                }
            }
        }

        this->on_viewport_resize(canvas_->width(), canvas_->height());

        { // Registra Camera controllers ViewProjection deve ser localizado acima
            auto view1 = registry_.get()->view<CameraComponent>();
            for (auto entity : view1) {
                Entity e(entity);

                auto& cc = e.get_component<CameraComponent>(registry_.get());
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
        scene_aabb_.set_boundary(tot_min, tot_max);
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

        create_octree(scene_aabb_);
    }

    void Scene::on_viewport_resize(const uint32_t& width, const uint32_t& height) {

        create_render_buffer(vpo_->get_size(), width, height);

        auto view = registry_.get()->view<CameraComponent>();
        for (auto entity : view) {
            auto& camera_component = view.get<CameraComponent>(entity);
            if (!camera_component.fixedAspectRatio) {

                for (auto render_buffer : v_rb_) { // altera a matrix de projecao apenas na troca de resolucao
                    camera_component.camera->set_viewport_size(render_buffer->width(), render_buffer->height());
                    if (camera_component.primary) {
                        active_cam_ = camera_component.camera;
                    }
                }
            }
        }
    }

    void Scene::on_event(const SDL_Event& event) {

        bool gotcha{true};

        switch (event.type) {
            case SDL_EVENT_WINDOW_RESIZED: {
                on_viewport_resize(event.window.data1, event.window.data2);
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

    void Scene::render_shadow(IRenderer3d& renderer) {

        renderer.begin(active_cam_, vpo_, nullptr);
        {
            auto light_view_ent = registry_.get()->view<LightComponent>();
            for (auto entity : light_view_ent) {
                auto& lc = light_view_ent.get<LightComponent>(entity);
                auto& tc = registry_.get()->get<TransComponent>(entity); // Lento
                if (lc.global) {
                    // FIXME: usar o direcionm depois no segundo parametro
                    glm::mat4 light_view =
                        glm::lookAt(tc.trans->get_position(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
                    shadow_data_.lightSpaceMatrix = shadow_data_.lightProjection * light_view;
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

    void Scene::exec_emitter_pass(IRenderer3d& renderer) {
        auto view = registry_.get()->view<RenderableParticlesComponent>();
        for (auto entity : view) {
            RenderableParticlesComponent& rc = view.get<RenderableParticlesComponent>(entity);
            Renderable3D* renderable = rc.renderable;

            Entity e(entity);
            TransComponent& tc = e.get_component<TransComponent>(registry_.get()); // FIXME: group this!!!
            auto& sc = e.get_component<ShaderComponent>(registry_.get());
            MaterialComponent& mc = e.get_component<MaterialComponent>(registry_.get());

            RenderCommand command;
            command.transform = tc.trans->translate_src(origem_->get_position());
            command.shader = sc.shader;
            mc.material->bind_material_information(command.uniforms, command.vTex);

            const glm::mat4& view = vpo_->get_sel().view;
            command.uniforms["projection"] = Uniform(renderer.get_camera()->get_projection());
            command.uniforms["view"] = Uniform(view);
            command.uniforms["CameraRight_worldspace"] = Uniform(glm::vec3(view[0][0], view[1][0], view[2][0]));
            command.uniforms["CameraUp_worldspace"] = Uniform(glm::vec3(view[0][1], view[1][1], view[2][1]));
            command.uniforms["model"] = Uniform(command.transform);
            renderable->submit(command, renderer);
        }
    }

    void Scene::exec_render_pass(IRenderer3d& renderer) {
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
            render_shadow(renderer);

        uint8_t count = 0;
        for (auto render_buffer : v_rb_) {

            vpo_->set_index(count);
            count++;

            // data load used by all
            renderer.ubo_queue().insert(std::make_pair("projection", Uniform(active_cam_->get_projection())));
            renderer.ubo_queue().insert(std::make_pair("view", Uniform(vpo_->get_sel().view)));

            // data load shadows props to renderer in shade of models!!!!
            if (shadow_data_.shadowBuffer) {
                renderer.ubo_queue().insert(std::make_pair("viewPos", Uniform(active_cam_->get_position())));
                renderer.ubo_queue().insert(std::make_pair("shadows", Uniform(1)));
                renderer.ubo_queue().insert(std::make_pair("shadowMap", Uniform(1)));
                renderer.ubo_queue().insert(std::make_pair("lightSpaceMatrix", Uniform(shadow_data_.lightSpaceMatrix)));
                renderer.tex_queue().push_back(shadow_data_.shadowBuffer->get_depth_attachemnt());
            }

            // data load lights
            auto light_view = registry_.get()->view<LightComponent>();
            for (auto entity : light_view) {
                auto& lc = light_view.get<LightComponent>(entity);
                auto& tc = registry_.get()->get<TransComponent>(entity); // lightView.get<LightComponent>(entity);
                if (lc.global) {                                         // biding light prop
                    lc.light->bind_light(renderer.ubo_queue(), tc.trans->get_matrix());
                }
            }

            render_buffer->bind(); // bind renderbuffer to draw we're not using the stencil buffer now

            renderer.begin(active_cam_, vpo_, octree_);
            this->exec_render_pass(renderer);
            renderer.end();
            renderer.flush();

            if (emitters_.size() > 0) {
                // inicializa state machine do opengl
                BinaryStateEnable depth(GL_DEPTH_TEST, GL_TRUE);
                BinaryStateEnable blender(GL_BLEND, GL_TRUE);
                DepthFuncSetter depth_func(GL_LESS); // Accept fragment if it closer to the camera than the former one
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                renderer.begin(active_cam_, vpo_, nullptr);
                this->exec_emitter_pass(renderer);
                renderer.end();
                renderer.flush();
            }

            if (verbose_ > 0) {      // RENDER DEBUG
                if (verbose_ == 1) { // DEBUG OCTREE

                    if (!dl_.valid()) {
                        std::unordered_map<GLenum, std::string> shade_data;
                        shade_data[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
                        shade_data[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";

                        auto assets = this->registry_->ctx().get<std::shared_ptr<AssetManager>>();

                        dl_.create(assets->load_shader("DrawLine", shade_data).handle(), 40000);
                    }

                    if (octree_ != nullptr) {

                        std::vector<AABB> list;
                        octree_->get_bondary_list(list, false);

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
                        std::unordered_map<GLenum, std::string> shade_data;
                        shade_data[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
                        shade_data[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";

                        auto assets = this->registry_->ctx().get<std::shared_ptr<AssetManager>>();

                        render_lines_.create(assets->load_shader("DrawLine", shade_data).handle(), 10000);
                    }

                    render_lines_.begin(active_cam_, vpo_, nullptr);

                    render_lines_.ubo_queue().insert(
                        std::make_pair("projection", Uniform(active_cam_->get_projection())));
                    render_lines_.ubo_queue().insert(std::make_pair("view", Uniform(vpo_->get_sel().view)));

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
                        TransComponent& tc = e.get_component<TransComponent>(registry_.get()); // FIXME: group this!!!

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

            render_buffer->unbind();
            render_buffer->render();
        }
    }
} // namespace ce
