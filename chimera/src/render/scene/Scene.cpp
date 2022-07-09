#include "chimera/render/scene/Scene.hpp"
#include "chimera/core/ScriptableEntity.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/visible/CameraControllerFPS.hpp"
#include "chimera/core/visible/CameraControllerOrbit.hpp"
#include "chimera/core/visible/Light.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/RenderCommand.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/render/3d/RenderableParticles.hpp"
#include "chimera/render/3d/Renderer3d.hpp"
#include "chimera/render/VertexData.hpp"
#include "chimera/render/scene/Components.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Scene::Scene(Registry& r, StateStack& s) : stack(&s), registry(&r), activeCam(nullptr), origem(nullptr), logRender(false) {}

Scene::~Scene() {
    if (shadowData.shadowBuffer) {
        delete shadowData.shadowBuffer;
        shadowData.shadowBuffer = nullptr;
    }
}

RenderBuffer* Scene::initRB(const uint32_t& initW, const uint32_t& initH, const uint32_t& width, const uint32_t& height) {
    // Define o framebuffer de desenho
    Shader shader;
    std::unordered_map<GLenum, std::string> shadeData;
    shadeData[GL_FRAGMENT_SHADER] = "./assets/shaders/CanvasHMD.frag";
    shadeData[GL_VERTEX_SHADER] = "./assets/shaders/CanvasHMD.vert";
    ShaderManager::load("CanvasHMD", shadeData, shader);

    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexFilter::LINEAR, TexWrap::CLAMP, TexWrap::CLAMP, TexWrap::CLAMP,
                 TexDType::UNSIGNED_BYTE),
        //   TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE,
        //            TexWrap::CLAMP_TO_EDGE, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexFilter::NONE, TexWrap::NONE, TexWrap::NONE,
                 TexWrap::NONE, TexDType::UNSIGNED_BYTE)};

    fbSpec.width = width;
    fbSpec.height = height;

    return new RenderBuffer(initW, initH, new FrameBuffer(fbSpec), shader);
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
    // destroy scripts
    registry->get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {
            nsc.instance->onDestroy();
            nsc.destroyScript(&nsc);
        }
    });
}

void Scene::onAttach() {
    // lista as tags nas entidades registradas
    registry->get().each([&](auto entityID) {
        Entity entity{entityID, registry};
        auto& tc = entity.getComponent<TagComponent>();
        SDL_Log("Tag: %s Id: %s", tc.tag.c_str(), tc.id.c_str());

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
                rc.renderable = new Renderable3D(mesh.mesh);
            else if (mesh.type == MeshType::ARRAY)
                rc.renderable = new RenderableArray(mesh.vTrisIndex, mesh.mesh);
            else if (mesh.type == MeshType::BSTREE)
                rc.renderable = new RenderableBsp(mesh.mesh);

            // Ajuste de fisica se existir
            if (entity.hasComponent<TransComponent>()) {
                glm::vec3 min, max, size;
                meshMinMaxSize(mesh.mesh, min, max, size);
                TransComponent& tc = entity.getComponent<TransComponent>();
                if (tc.solid) {
                    Solid* solid = (Solid*)tc.trans;
                    solid->init(size); // Cria rigidBody iniciaza transformacao e inicializa shape se ele nao existir
                }
            }
        }

        // Se existir particulas
        if (entity.hasComponent<EmitterComponent>()) {
            EmitterComponent& ec = entity.getComponent<EmitterComponent>();
            if (!entity.hasComponent<RenderableParticlesComponent>()) {
                RenderableParticlesComponent& particleSys = entity.addComponent<RenderableParticlesComponent>();
                particleSys.enable = true;
                RenderableParticles* p = new RenderableParticles();
                ParticleContainer* pc = ec.emitter->getContainer(0); // FIXME: melhorar!!!!
                p->setParticleContainer(pc);
                p->create();
                particleSys.renderable = p;
                emitters.push_back(ec.emitter);
            }
        }

        if (entity.hasComponent<FrameBufferSpecification>()) {
            if (tc.tag == "shadow01") { // init shadow data
                FrameBufferSpecification& fbSpec = entity.getComponent<FrameBufferSpecification>();
                CameraComponent& cc = entity.getComponent<CameraComponent>();
                cc.camera->setViewportSize(fbSpec.width, fbSpec.height);
                shadowData.shader = entity.getComponent<Shader>();
                shadowData.lightProjection = cc.camera->getProjection();
                shadowData.shadowBuffer = new FrameBuffer(fbSpec);
            }
        }

        // Pega o EyeView do ECS
        if (entity.hasComponent<EyeView>()) {
            EyeView& ev = entity.getComponent<EyeView>();
            eyeView = &ev;
        }

        // Pega o Canvas do ECS
        if (entity.hasComponent<CanvasComponent>()) {
            CanvasComponent& cc = entity.getComponent<CanvasComponent>();
            this->onViewportResize(cc.canvas->getWidth(), cc.canvas->getHeight());
        }
    });

    {
        // Registra Camera controllers EyeView deve ser localizado acima
        auto view1 = registry->get().view<CameraComponent>();
        for (auto entity : view1) {
            Entity e = Entity{entity, registry};

            auto& cc = e.getComponent<CameraComponent>();
            cc.eyeView = eyeView;
            if (cc.camKind == CamKind::FPS) {
                e.addComponent<NativeScriptComponent>().bind<CameraControllerFPS>("CameraController");
            } else if (cc.camKind == CamKind::ORBIT) {
                e.addComponent<NativeScriptComponent>().bind<CameraControllerOrbit>("CameraController");
            } else if (cc.camKind == CamKind::STATIC) {
                //     e.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
            }
        }
    }

    {
        // initialize scripts
        registry->get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
            if (!nsc.instance) {
                nsc.instance = nsc.instantiateScript();
                nsc.instance->entity = Entity{entity, registry};
                nsc.instance->onCreate();
            }
        });
    }

    origem = new Transform(); // FIXME: coisa feia!!!!
}

Canvas* Scene::getCanvas() {
    CanvasComponent& cc = registry->findComponent<CanvasComponent>("main_screem");
    return cc.canvas;
}

void Scene::onUpdate(const double& ts) {
    // update scripts (PhysicController aqui dentro!!!)
    registry->get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (nsc.instance)
            nsc.instance->onUpdate(ts);
    });

    for (auto emissor : emitters)
        emissor->recycleLife(ts);
}

void Scene::onViewportResize(const uint32_t& width, const uint32_t& height) {

    auto view = registry->get().view<CameraComponent>();
    for (auto entity : view) {

        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.fixedAspectRatio) {
            cameraComponent.camera->setViewportSize(width, height);

            // carrega camera defalt da cena
            if (cameraComponent.primary == true) {
                createRenderBuffer(eyeView->size(), width, height);
                activeCam = cameraComponent.camera;
            }
        }
    }
}

bool Scene::onEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED: // aqui para camera e engine acerta a janela
                    onViewportResize(event.window.data1, event.window.data2);
                    break;
            }
        } break;
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_F9:
                    logRender = !logRender;
                    break;
            }
        } break;
    }
    return true;
}

void Scene::renderShadow(Camera* camera, IRenderer3d& renderer, ITrans* origem) {

    renderer.begin(camera, eyeView);
    {
        auto lightViewEnt = registry->get().view<LightComponent>();
        for (auto entity : lightViewEnt) {
            auto& lc = lightViewEnt.get<LightComponent>(entity);
            auto& tc = registry->get().get<TransComponent>(entity); // Lento
            if (lc.global) {
                // FIXME: usar o direcionm depois no segundo parametro
                glm::mat4 lightView = glm::lookAt(tc.trans->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
                shadowData.lightSpaceMatrix = shadowData.lightProjection * lightView;
            }
        }

        auto group = registry->get().group<TransComponent, Renderable3dComponent>();
        for (auto entity : group) {
            auto [tc, rc] = group.get<TransComponent, Renderable3dComponent>(entity);

            RenderCommand command;
            command.transform = tc.trans->translateSrc(origem->getPosition());
            command.shader = shadowData.shader;
            command.uniforms["model"] = UValue(command.transform);
            command.uniforms["lightSpaceMatrix"] = UValue(shadowData.lightSpaceMatrix);
            rc.renderable->submit(command, renderer);
        }
    }

    renderer.end();
    shadowData.shadowBuffer->bind(); // we're not using the stencil buffer now

    renderer.flush();
    shadowData.shadowBuffer->unbind();
}

void Scene::execEmitterPass(IRenderer3d& renderer) {
    auto view = registry->get().view<RenderableParticlesComponent>();
    for (auto entity : view) {
        RenderableParticlesComponent& rc = view.get<RenderableParticlesComponent>(entity);
        IRenderable3d* renderable = rc.renderable;

        Entity e = {entity, registry};
        TransComponent& tc = e.getComponent<TransComponent>(); // FIXME: group this!!!
        Shader& sc = e.getComponent<Shader>();
        MaterialComponent& mc = e.getComponent<MaterialComponent>();

        RenderCommand command;
        command.transform = tc.trans->translateSrc(origem->getPosition());
        command.shader = sc;
        mc.material->bindMaterialInformation(command.uniforms, command.vTex);

        const glm::mat4& view = eyeView->getView();
        command.uniforms["projection"] = UValue(renderer.getCamera()->getProjection());
        command.uniforms["view"] = UValue(view);
        command.uniforms["CameraRight_worldspace"] = UValue(glm::vec3(view[0][0], view[1][0], view[2][0]));
        command.uniforms["CameraUp_worldspace"] = UValue(glm::vec3(view[0][1], view[1][1], view[2][1]));
        command.uniforms["model"] = UValue(command.transform);
        renderable->submit(command, renderer);
    }
}

void Scene::execRenderPass(IRenderer3d& renderer) {
    auto group = registry->get().group<Shader, MaterialComponent, TransComponent, Renderable3dComponent>();
    for (auto entity : group) {
        auto [sc, mc, tc, rc] = group.get<Shader, MaterialComponent, TransComponent, Renderable3dComponent>(entity);

        RenderCommand command;
        command.transform = tc.trans->translateSrc(origem->getPosition());
        command.shader = sc;
        mc.material->bindMaterialInformation(command.uniforms, command.vTex);
        command.uniforms["model"] = UValue(command.transform);
        rc.renderable->submit(command, renderer);
    }
}

void Scene::onRender() {

    Camera* camera = activeCam;
    Renderer3d renderBatch(logRender);

    if (logRender == true) {
        const glm::vec3& pos = camera->getPosition();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    }

    // render a shadows in framebuffer
    if (shadowData.shadowBuffer)
        renderShadow(camera, renderBatch, origem);

    uint8_t count = 0;
    for (auto renderBuffer : vRB) {
        camera->setViewportSize(renderBuffer->getWidth(), renderBuffer->getHeight()); // TODO: se camera nao muda projecao rodar apenas 1
        eyeView->setIndex(count);
        count++;

        // data load used by all
        renderBatch.uboQueue().insert(std::make_pair("projection", UValue(camera->getProjection())));
        renderBatch.uboQueue().insert(std::make_pair("view", UValue(eyeView->getView())));

        // data load shadows props to renderBatch in shade of models!!!!
        if (shadowData.shadowBuffer) {
            renderBatch.uboQueue().insert(std::make_pair("viewPos", UValue(camera->getPosition())));
            renderBatch.uboQueue().insert(std::make_pair("shadows", UValue(1)));
            renderBatch.uboQueue().insert(std::make_pair("shadowMap", UValue(1)));
            renderBatch.uboQueue().insert(std::make_pair("lightSpaceMatrix", UValue(shadowData.lightSpaceMatrix)));
            renderBatch.texQueue().push_back(shadowData.shadowBuffer->getDepthAttachemnt());
        }

        // data load lights
        auto lightView = registry->get().view<LightComponent>();
        for (auto entity : lightView) {
            auto& lc = lightView.get<LightComponent>(entity);
            auto& tc = registry->get().get<TransComponent>(entity); // lightView.get<LightComponent>(entity);
            if (lc.global)                                          // biding light prop
                lc.light->bindLight(renderBatch.uboQueue(), tc.trans->getMatrix());
        }

        renderBuffer->bind(); // bind renderbuffer to draw we're not using the stencil buffer now

        renderBatch.begin(camera, eyeView);
        this->execRenderPass(renderBatch);
        renderBatch.end();
        renderBatch.flush();

        if (emitters.size() > 0) {
            // inicializa state machine do opengl
            BinaryStateEnable depth(GL_DEPTH_TEST);
            BinaryStateEnable blender(GL_BLEND);
            DepthFuncSetter depthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            renderBatch.begin(camera, eyeView);
            this->execEmitterPass(renderBatch);
            renderBatch.end();
            renderBatch.flush();
        }

        {
            // TODO: captura do entity no framebuffer da tela
            // get val from color buffer (must be inside framebuffer renderer
            // glm::ivec2 pos = MouseDevice::getMove();
            // pos.y = viewportHeight - pos.y;
            // int val = renderBuffer->getFramBuffer()->readPixel(1, pos.x, pos.y);
            // SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "mouse(X: %d / Y: %d): %d", pos.x, pos.y, val);
        }

        renderBuffer->unbind();
        renderBuffer->render();
    }
}
} // namespace Chimera