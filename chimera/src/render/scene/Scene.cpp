#include "chimera/render/scene/Scene.hpp"
#include "chimera/base/Mouse.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/core/gl/RenderCommand.hpp"
#include "chimera/core/gl/ShaderMng.hpp"
#include "chimera/core/gl/buffer/VertexArray.hpp"
#include "chimera/core/visible/CameraControllerFPS.hpp"
#include "chimera/core/visible/CameraControllerOrbit.hpp"
#include "chimera/ecs/CameraComponent.hpp"
#include "chimera/ecs/EmitterComponent.hpp"
#include "chimera/ecs/LightComponent.hpp"
#include "chimera/ecs/MaterialComponent.hpp"
#include "chimera/ecs/MeshComponent.hpp"
#include "chimera/ecs/Renderable3dComponent.hpp"
#include "chimera/ecs/ShaderComponent.hpp"
#include "chimera/ecs/TransComponent.hpp"
#include "chimera/render/2d/Tile.hpp"
#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/render/3d/RenderableMesh.hpp"
#include "chimera/render/3d/RenderableParticles.hpp"
#include "chimera/render/3d/Renderer3d.hpp"

namespace ce {

    Scene::Scene() : IStateMachine("Scene"), origem(nullptr), verbose(0) { octree = nullptr; }

    Scene::~Scene() {
        if (shadowData.shadowBuffer) {
            delete shadowData.shadowBuffer;
            shadowData.shadowBuffer = nullptr;
        }
    }

    RenderBuffer* Scene::initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width,
                                const uint32_t& height) {
        if (!eRenderBuferSpec)
            throw std::string("RenderBuffer nao encontrado");

        // Define o framebuffer de desenho
        FrameBufferSpecification& fbSpec = eRenderBuferSpec.getComponent<FrameBufferSpecification>();
        fbSpec.width = width;
        fbSpec.height = height;
        auto& sc = eRenderBuferSpec.getComponent<ShaderComponent>();
        return new RenderBuffer(initW, initH, std::make_shared<FrameBuffer>(fbSpec), sc.shader);
    }

    void Scene::createRenderBuffer(const uint8_t& size, const uint32_t& width, const uint32_t& height) {

        for (auto rb : vRB) {
            delete rb;
            rb = nullptr;
        }

        vRB.clear();
        uint32_t halfHidth = width / 2;
        if (size == 2) {
            vRB.push_back(initRB(0, 0, halfHidth, height));         // left
            vRB.push_back(initRB(halfHidth, 0, halfHidth, height)); // right
        } else {
            vRB.push_back(initRB(0, 0, width, height)); // full only
        }
    }

    void Scene::onDeatach() {
        vpo = nullptr;
        phyCrt = nullptr;
    }

    void Scene::createOctree(const AABB& aabb) {
        if (octree != nullptr) {
            delete octree;
        }
        octree = new Octree(aabb, 27, true); // 18
    }

    void Scene::onAttach() {
        // Pega o ViewProjection do ECS antes da camera por caussa do vpo
        vpo = g_service_locator.getService<ViewProjection>();
        phyCrt = g_service_locator.getServiceOrNull<IPhysicsControl>();

        // Totalizadores de area
        glm::vec3 tot_min, tot_max;
        int tot_mesh = 0;

        // lista as tags nas entidades registradas
        for (auto entityID : g_registry.get().view<entt::entity>()) {
            Entity entity(entityID);
            auto& tc = entity.getComponent<TagInfo>();
            SDL_Log("Tag: %s Id: %s", tc.name.c_str(), tc.id.c_str());

            if (tc.name == "TileText") {
                CameraComponent& cCam = entity.getComponent<CameraComponent>();
                auto& sc = entity.getComponent<ShaderComponent>();
                // TileComponent& tc = entity.addComponent<TileComponent>();
                Tile* tile = new Tile("TileText", &batchRender2D, sc.shader,
                                      cCam.camera.get()); // TODO: passar tile camera para smart
                layers.pushState(tile);
            }

            // Se for um mesh inicializar componente
            if (entity.hasComponent<MeshComponent>()) {
                MeshComponent& mesh = entity.getComponent<MeshComponent>();

                // Inicializa Materiais
                if (entity.hasComponent<MaterialComponent>()) {
                    MaterialComponent& material = entity.getComponent<MaterialComponent>();
                    if (!material.material->isValid())
                        material.material->init();
                } else {
                    MaterialComponent& material = entity.addComponent<MaterialComponent>();
                    material.material->setDefaultEffect();
                    material.material->init();
                }

                // Cria componentes renderizaveis
                Renderable3dComponent& rc = entity.addComponent<Renderable3dComponent>();
                if (mesh.type == MeshType::SIMPLE)
                    rc.renderable = new RenderableMesh(mesh.mesh);
                else if (mesh.type == MeshType::ARRAY)
                    rc.renderable = new RenderableArray(mesh.vTrisIndex, mesh.mesh);
                else if (mesh.type == MeshType::BSTREE)
                    rc.renderable = new RenderableBsp(*mesh.mesh);

                auto [min, max, size] = vertexBoundaries(mesh.mesh->vertex);

                if (entity.hasComponent<TransComponent>()) {
                    // Ajuste de fisica se existir
                    TransComponent& tc = entity.getComponent<TransComponent>();
                    if (tc.solid) {
                        // Cria rigidBody iniciaza transformacao e inicializa shape se ele nao existir
                        Solid* solid = (Solid*)tc.trans;
                        // TODO: Era half size ??
                        solid->init(size / 2.0f);
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
            if (entity.hasComponent<EmitterComponent>()) {
                EmitterComponent& ec = entity.getComponent<EmitterComponent>();
                if (!entity.hasComponent<RenderableParticlesComponent>()) {
                    RenderableParticlesComponent& particleSys = entity.addComponent<RenderableParticlesComponent>();
                    particleSys.enable = true;
                    RenderableParticles* p = new RenderableParticles();
                    std::shared_ptr<ParticleContainer> pc = ec.emitter->getContainer(0); // FIXME: melhorar!!!!
                    p->setParticleContainer(pc);
                    p->create();
                    particleSys.renderable = p;
                    emitters.push_back(ec.emitter);
                }
            }

            if (entity.hasComponent<FrameBufferSpecification>()) {
                FrameBufferSpecification& fbSpec = entity.getComponent<FrameBufferSpecification>();
                if (tc.name == "shadow01") { // init shadow data

                    auto& sc = entity.getComponent<ShaderComponent>();
                    CameraComponent& cc = entity.getComponent<CameraComponent>();
                    cc.camera->setViewportSize(fbSpec.width, fbSpec.height);
                    shadowData.shader = sc.shader; // entity.getComponent<Shader>();
                    shadowData.lightProjection = cc.camera->getProjection();
                    shadowData.shadowBuffer = new FrameBuffer(fbSpec);
                } else if (tc.name == "RenderBufferMaster") {
                    eRenderBuferSpec = entity;
                }
            }
        }

        // Pega icanvas depois de camera definida!!!
        auto canvas = g_service_locator.getService<ICanva>();
        this->onViewportResize(canvas->getWidth(), canvas->getHeight());

        { // Registra Camera controllers ViewProjection deve ser localizado acima
            auto view1 = g_registry.get().view<CameraComponent>();
            for (auto entity : view1) {
                Entity e(entity);

                auto& cc = e.getComponent<CameraComponent>();
                if (cc.camKind == CamKind::FPS) {
                    // CameraControllerFPS* ccFps = new CameraControllerFPS(e);
                    layers.pushState(new CameraControllerFPS(e));
                } else if (cc.camKind == CamKind::ORBIT) {
                    // CameraControllerOrbit* ccOrb = new CameraControllerOrbit(e);
                    layers.pushState(new CameraControllerOrbit(e));
                } else if (cc.camKind == CamKind::STATIC) {
                    // e.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
                }
            }
        }

        origem = new Transform(); // FIXME: coisa feia!!!!
        sceneAABB.setBoundary(tot_min, tot_max);
    }

    void Scene::onUpdate(const double& ts) {

        if (phyCrt) {
            phyCrt->stepSim(ts);
            phyCrt->checkCollisions();
        }

        for (auto emissor : emitters)
            emissor->recycleLife(ts);

        for (auto it = layers.begin(); it != layers.end(); it++)
            (*it)->onUpdate(ts);

        createOctree(sceneAABB);
    }

    void Scene::onViewportResize(const uint32_t& width, const uint32_t& height) {

        createRenderBuffer(vpo->getSize(), width, height);

        auto view = g_registry.get().view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.fixedAspectRatio) {

                for (auto renderBuffer : vRB) { // altera a matrix de projecao apenas na troca de resolucao
                    cameraComponent.camera->setViewportSize(renderBuffer->getWidth(), renderBuffer->getHeight());
                    if (cameraComponent.primary == true) {
                        activeCam = cameraComponent.camera;
                    }
                }
            }
        }
    }

    bool Scene::onEvent(const SDL_Event& event) {
        switch (event.type) {

            case SDL_EVENT_WINDOW_RESIZED: {
                onViewportResize(event.window.data1, event.window.data2);
            } break;
            case SDL_EVENT_KEY_DOWN: {
                switch (event.key.key) {
                    case SDLK_F9: {
                        verbose++;
                        if (verbose > 2)
                            verbose = 0;

                    } break;
                }
            } break;
        }

        for (auto it = layers.begin(); it != layers.end(); it++)
            (*it)->onEvent(event);

        return true;
    }

    void Scene::renderShadow(IRenderer3d& renderer) {

        renderer.begin(activeCam, vpo.get(), nullptr);
        {
            auto lightViewEnt = g_registry.get().view<LightComponent>();
            for (auto entity : lightViewEnt) {
                auto& lc = lightViewEnt.get<LightComponent>(entity);
                auto& tc = g_registry.get().get<TransComponent>(entity); // Lento
                if (lc.global) {
                    // FIXME: usar o direcionm depois no segundo parametro
                    glm::mat4 lightView =
                        glm::lookAt(tc.trans->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
                    shadowData.lightSpaceMatrix = shadowData.lightProjection * lightView;
                }
            }

            auto group = g_registry.get().group<TransComponent, Renderable3dComponent>();
            for (auto entity : group) {
                auto [tc, rc] = group.get<TransComponent, Renderable3dComponent>(entity);

                RenderCommand command;
                command.transform = tc.trans->translateSrc(origem->getPosition());
                command.shader = shadowData.shader;
                command.uniforms["model"] = Uniform(command.transform);
                command.uniforms["lightSpaceMatrix"] = Uniform(shadowData.lightSpaceMatrix);
                rc.renderable->submit(command, renderer);
            }
        }

        renderer.end();
        shadowData.shadowBuffer->bind(); // we're not using the stencil buffer now

        renderer.flush();
        shadowData.shadowBuffer->unbind();
    }

    void Scene::execEmitterPass(IRenderer3d& renderer) {
        auto view = g_registry.get().view<RenderableParticlesComponent>();
        for (auto entity : view) {
            RenderableParticlesComponent& rc = view.get<RenderableParticlesComponent>(entity);
            Renderable3D* renderable = rc.renderable;

            Entity e(entity);
            TransComponent& tc = e.getComponent<TransComponent>(); // FIXME: group this!!!
            auto& sc = e.getComponent<ShaderComponent>();
            MaterialComponent& mc = e.getComponent<MaterialComponent>();

            RenderCommand command;
            command.transform = tc.trans->translateSrc(origem->getPosition());
            command.shader = sc.shader;
            mc.material->bindMaterialInformation(command.uniforms, command.vTex);

            const glm::mat4& view = vpo->getSel().view;
            command.uniforms["projection"] = Uniform(renderer.getCamera()->getProjection());
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
        auto group = g_registry.get().view<ShaderComponent, MaterialComponent, TransComponent, Renderable3dComponent>();
        for (auto entity : group) {
            auto [sc, mc, tc, rc] =
                group.get<ShaderComponent, MaterialComponent, TransComponent, Renderable3dComponent>(entity);

            RenderCommand command;
            command.transform = tc.trans->translateSrc(origem->getPosition());
            command.shader = sc.shader;
            mc.material->bindMaterialInformation(command.uniforms, command.vTex);
            command.uniforms["model"] = Uniform(command.transform);
            rc.renderable->submit(command, renderer);
        }
    }

    void Scene::onRender() {
        Renderer3d renderer(verbose > 0);

        if (verbose > 0) {
            const glm::vec3& pos = activeCam->getPosition();
            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
        }

        // render a shadows in framebuffer
        if (shadowData.shadowBuffer)
            renderShadow(renderer);

        uint8_t count = 0;
        for (auto renderBuffer : vRB) {

            vpo->setIndex(count);
            count++;

            // data load used by all
            renderer.uboQueue().insert(std::make_pair("projection", Uniform(activeCam->getProjection())));
            renderer.uboQueue().insert(std::make_pair("view", Uniform(vpo->getSel().view)));

            // data load shadows props to renderer in shade of models!!!!
            if (shadowData.shadowBuffer) {
                renderer.uboQueue().insert(std::make_pair("viewPos", Uniform(activeCam->getPosition())));
                renderer.uboQueue().insert(std::make_pair("shadows", Uniform(1)));
                renderer.uboQueue().insert(std::make_pair("shadowMap", Uniform(1)));
                renderer.uboQueue().insert(std::make_pair("lightSpaceMatrix", Uniform(shadowData.lightSpaceMatrix)));
                renderer.texQueue().push_back(shadowData.shadowBuffer->getDepthAttachemnt());
            }

            // data load lights
            auto lightView = g_registry.get().view<LightComponent>();
            for (auto entity : lightView) {
                auto& lc = lightView.get<LightComponent>(entity);
                auto& tc = g_registry.get().get<TransComponent>(entity); // lightView.get<LightComponent>(entity);
                if (lc.global)                                           // biding light prop
                    lc.light->bindLight(renderer.uboQueue(), tc.trans->getMatrix());
            }

            renderBuffer->bind(); // bind renderbuffer to draw we're not using the stencil buffer now

            renderer.begin(activeCam, vpo.get(), octree);
            this->execRenderPass(renderer);
            renderer.end();
            renderer.flush();

            if (emitters.size() > 0) {
                // inicializa state machine do opengl
                BinaryStateEnable depth(GL_DEPTH_TEST, GL_TRUE);
                BinaryStateEnable blender(GL_BLEND, GL_TRUE);
                DepthFuncSetter depthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                renderer.begin(activeCam, vpo.get(), nullptr);
                this->execEmitterPass(renderer);
                renderer.end();
                renderer.flush();
            }

            if (verbose > 0) {      // RENDER DEBUG
                if (verbose == 1) { // DEBUG OCTREE

                    if (dl.valid() == false) {
                        std::unordered_map<GLenum, std::string> shadeData;
                        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
                        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";

                        auto mng = g_service_locator.getService<ShaderMng>();
                        dl.create(mng->load("DrawLine", shadeData), 40000);
                    }

                    if (octree != nullptr) {
                        std::vector<AABB> list;
                        octree->getBondaryList(list, false);

                        for (auto& aabb : list) {
                            dl.addAABB(aabb, glm::vec3(1.0, 1.0, 1.0));
                        }

                        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Octree Size: %ld", list.size());

                        MapUniform muni;
                        muni["projection"] = Uniform(activeCam->getProjection());
                        muni["view"] = Uniform(vpo->getSel().view);
                        dl.render(muni);
                    }

                } else if (verbose == 2) { // DEBUG AABB

                    if (renderLines.valid() == false) {
                        std::unordered_map<GLenum, std::string> shadeData;
                        shadeData[GL_VERTEX_SHADER] = "./assets/shaders/Line.vert";
                        shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/Line.frag";

                        auto mng = g_service_locator.getService<ShaderMng>();
                        renderLines.create(mng->load("DrawLine", shadeData), 10000);
                    }

                    renderLines.begin(activeCam, vpo.get(), nullptr);

                    renderLines.uboQueue().insert(std::make_pair("projection", Uniform(activeCam->getProjection())));
                    renderLines.uboQueue().insert(std::make_pair("view", Uniform(vpo->getSel().view)));

                    auto group = g_registry.get().group<TransComponent, Renderable3dComponent>();
                    for (auto entity : group) {
                        auto [tc, rc] = group.get<TransComponent, Renderable3dComponent>(entity);

                        RenderCommand command;
                        command.transform = tc.trans->translateSrc(origem->getPosition());
                        rc.renderable->submit(command, renderLines);
                    }

                    auto view = g_registry.get().view<RenderableParticlesComponent>();
                    for (auto entity : view) {
                        RenderableParticlesComponent& rc = view.get<RenderableParticlesComponent>(entity);
                        Renderable3D* renderable = rc.renderable;

                        Entity e(entity);
                        TransComponent& tc = e.getComponent<TransComponent>(); // FIXME: group this!!!

                        RenderCommand command;
                        command.transform = tc.trans->translateSrc(origem->getPosition());

                        renderable->submit(command, renderLines);
                    }

                    renderLines.end();
                    renderLines.flush();
                }
            }

            for (auto it = layers.begin(); it != layers.end(); it++)
                (*it)->onRender();

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
