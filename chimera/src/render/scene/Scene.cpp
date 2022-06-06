#include "chimera/render/scene/Scene.hpp"
#include "chimera/core/ScriptableEntity.hpp"
#include "chimera/core/buffer/VertexArray.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/partition/BSPTree.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/Transform.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/3d/Renderable3D.hpp"
#include "chimera/render/3d/RenderableArray.hpp"
#include "chimera/render/3d/RenderableBsp.hpp"
#include "chimera/render/3d/RenderableParticles.hpp"
#include "chimera/render/VertexData.hpp"
#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Scene::Scene() : activeCam(nullptr), origem(nullptr), shadowPass(nullptr), logRender(false) {}

Scene::~Scene() {
    if (shadowPass) {
        delete shadowPass;
        shadowPass = nullptr;
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
        TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE),
        // TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)};

    fbSpec.width = width;
    fbSpec.height = height;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

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
    registry.get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {
            nsc.instance->onDestroy();
            nsc.destroyScript(&nsc);
        }
    });
}

void Scene::onAttach() {

    // lista as tags nas entidades registradas
    registry.get().each([&](auto entityID) {
        Entity entity{entityID, &registry};
        auto& tc = entity.getComponent<TagComponent>();
        SDL_Log("Tag: %s Id: %s", tc.tag.c_str(), tc.id.c_str());

        // Se for um mesh inicializar componente (já que nao tenho classe de Mesh)
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

            // Se nja nao foi inicializado um Renderable3dComponent ao mesh
            if (!entity.hasComponent<Renderable3dComponent>()) { // FIXME: Depoin do BSP mudar o teste ja que todos serao criados aqui!!!

                Renderable3dComponent& rc = entity.addComponent<Renderable3dComponent>();

                if (mesh.type == MeshType::SIMPLE) {

                    // Transforma Mesh em VertexData comprimindo-o
                    std::vector<VertexData> renderData;
                    vertexDataFromMesh(mesh.mesh, renderData);

                    std::vector<uint32_t> index;
                    std::vector<VertexData> vertexDataOut;
                    vertexDataIndexCompile(renderData, vertexDataOut, index);

                    // Create VAO, VBO and IBO
                    VertexArray* vao = new VertexArray();
                    vao->bind();

                    VertexBuffer* vbo = new VertexBuffer(BufferType::STATIC);
                    vbo->bind();

                    BufferLayout layout;
                    layout.push(3, GL_FLOAT, sizeof(float), false);
                    layout.push(3, GL_FLOAT, sizeof(float), false);
                    layout.push(2, GL_FLOAT, sizeof(float), false);

                    vbo->setLayout(layout);
                    vbo->setData(&vertexDataOut[0], vertexDataOut.size());
                    vbo->unbind();
                    vao->push(vbo);

                    vao->unbind();

                    glm::vec3 min, max, size;
                    vertexDataIndexMinMaxSize(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size(), min, max, size);

                    IndexBuffer* ibo = new IndexBuffer(&index[0], index.size());
                    Renderable3D* r = new Renderable3D(vao, ibo, AABB(min, max));

                    rc.renderable = r;
                } else if (mesh.type == MeshType::ARRAY) {

                    std::vector<VertexData> renderData;
                    vertexDataFromMesh(mesh.mesh, renderData);

                    rc.renderable = new RenderableArray(mesh.vTrisIndex, renderData);

                } else if (mesh.type == MeshType::BSTREE) {

                    Mesh meshFinal;
                    meshReCompile(*mesh.mesh, meshFinal);

                    // btree root, leafs, vertex
                    BspTree bspTree;
                    mesh.root = bspTree.create(&meshFinal, mesh.vTrisIndex);

                    rc.renderable = new RenderableBsp(mesh.root, mesh.vTrisIndex, &meshFinal);
                }
            }

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
                this->pushEmitters(ec.emitter);
            }
        }

        if (entity.hasComponent<CanvasComponent>()) {
            CanvasComponent& cc = entity.getComponent<CanvasComponent>();
            this->onViewportResize(cc.canvas->getWidth(), cc.canvas->getHeight());
        }

        if (entity.hasComponent<EyeView>()) {
            EyeView& ev = entity.getComponent<EyeView>();
            eyeView = &ev;
        }
    });

    {
        // Registra Camera controllers EyeView deve ser localizado acima
        auto view1 = this->getRegistry().get().view<CameraComponent>();
        for (auto entity : view1) {
            Entity e = Entity{entity, &this->getRegistry()};

            auto& cc = e.getComponent<CameraComponent>();
            cc.eyeView = eyeView;
            if (cc.d.camKind == CamKind::FPS) {
                e.addComponent<NativeScriptComponent>().bind<CameraControllerFPS>("CameraController");
            } else if (cc.d.camKind == CamKind::ORBIT) {
                e.addComponent<NativeScriptComponent>().bind<CameraControllerOrbit>("CameraController");
            }
            // if (cc.camera->is3D() == true)
            //     e.addComponent<NativeScriptComponent>().bind<CameraController>("CameraController");
        }
    }

    {
        // initialize scripts
        registry.get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
            if (!nsc.instance) {
                nsc.instance = nsc.instantiateScript();
                nsc.instance->entity = Entity{entity, &registry};
                nsc.instance->onCreate();
            }
        });
    }

    origem = new Transform(); // FIXME: coisa feia!!!!
}

Canvas* Scene::getCanvas() {
    CanvasComponent& cc = registry.findComponent<CanvasComponent>("main_canvas");
    return cc.canvas;
}

void Scene::onUpdate(const double& ts) {
    // update scripts (PhysicController aqui dentro!!!)
    registry.get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (nsc.instance)
            nsc.instance->onUpdate(ts);
    });

    for (auto emissor : emitters)
        emissor->recycleLife(ts);
}

void Scene::onViewportResize(const uint32_t& width, const uint32_t& height) {

    auto view = registry.get().view<CameraComponent>();
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

void Scene::execEmitterPass(ICamera* camera, IRenderer3d& renderer) {
    auto view = registry.get().view<RenderableParticlesComponent>();
    for (auto entity : view) {

        RenderableParticlesComponent& rc = view.get<RenderableParticlesComponent>(entity);
        IRenderable3d* renderable = rc.renderable;

        Entity e = {entity, &registry};
        TransComponent& tc = e.getComponent<TransComponent>(); // FIXME: group this!!!
        Shader& sc = e.getComponent<Shader>();
        MaterialComponent& mc = e.getComponent<MaterialComponent>();

        RenderCommand command;
        command.camera = camera;
        command.eyeView = eyeView;
        command.logRender = logRender;
        command.transform = tc.trans->translateSrc(origem->getPosition());
        command.renderable = renderable;
        command.shader = sc;
        mc.material->bindMaterialInformation(command.uniforms, command.vTex);

        command.uniforms["model"] = UValue(command.transform);

        renderable->submit(command, renderer);
    }
}

void Scene::execRenderPass(ICamera* camera, IRenderer3d& renderer) {
    auto group = registry.get().group<Shader, MaterialComponent, TransComponent, Renderable3dComponent>();
    for (auto entity : group) {
        auto [sc, mc, tc, rc] = group.get<Shader, MaterialComponent, TransComponent, Renderable3dComponent>(entity);

        RenderCommand command;
        command.camera = camera;
        command.eyeView = eyeView;
        command.logRender = logRender;
        command.transform = tc.trans->translateSrc(origem->getPosition());
        command.renderable = rc.renderable;
        command.shader = sc;
        mc.material->bindMaterialInformation(command.uniforms, command.vTex);
        command.uniforms["model"] = UValue(command.transform);
        rc.renderable->submit(command, renderer);
    }
}

void Scene::onRender() {

    ICamera* camera = activeCam;

    if (logRender == true) {
        const glm::vec3& pos = camera->getPosition();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    }

    // render a shadows in framebuffer
    if (shadowPass)
        shadowPass->exec(registry, camera, eyeView, renderBatch, origem, logRender);

    uint8_t count = 0;
    for (auto renderBuffer : vRB) {
        camera->setViewportSize(renderBuffer->getWidth(), renderBuffer->getHeight());
        eyeView->setIndex(count);
        count++;

        // used by all
        renderBatch.uQueue().insert(std::make_pair("projection", UValue(camera->getProjection())));
        renderBatch.uQueue().insert(std::make_pair("view", UValue(eyeView->getView())));

        // load shadows props to renderBatch
        if (shadowPass)
            shadowPass->appy(camera, renderBatch);

        auto lightView = registry.get().view<LightComponent>();
        for (auto entity : lightView) {
            auto& lc = lightView.get<LightComponent>(entity);
            auto& tc = registry.get().get<TransComponent>(entity); // lightView.get<LightComponent>(entity);
            if (lc.global)                                         // biding light prop
                lc.light->bindLight(renderBatch.uQueue(), tc.trans->getMatrix());
        }

        { // Render mesh
            renderBatch.begin(camera, eyeView);
            this->execRenderPass(camera, renderBatch);

            renderBatch.end();
            renderBuffer->bind(); // we're not using the stencil buffer now

            renderBatch.flush(); // desenha aqui!!!!
        }

        if (emitters.size() > 0) {
            // inicializa state machine do opengl
            BinaryStateEnable depth(GL_DEPTH_TEST); // glEnable(GL_DEPTH_TEST);// Enable depth test
            BinaryStateEnable blender(GL_BLEND);    // glEnable(GL_BLEND);
            DepthFuncSetter depthFunc(GL_LESS); // glDepthFunc(GL_LESS);   // Accept fragment if it closer to the camera than the former one
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            renderBatch.begin(camera, eyeView);
            this->execEmitterPass(camera, renderBatch);

            renderBatch.end();
            renderBatch.flush();
        }

        {
            // TODO: captura do entity no framebuffer da tela
            // // get val from color buffer (must be inside framebuffer renderer
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