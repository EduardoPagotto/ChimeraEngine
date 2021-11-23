#include "chimera/render/scene/Scene.hpp"
#include "chimera/core/MouseDevice.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/3d/RenderableParticles.hpp"
#include "chimera/render/3d/RenderableSimple.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/Transform.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/bullet/Solid.hpp"
#include "chimera/render/scene/Components.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Scene::Scene() : camera(nullptr), viewportWidth(800), viewportHeight(640), renderBuffer(nullptr), physicsControl(nullptr) {
    // Create ShadowPass
    ShaderManager::load("./assets/shaders/ShadowMappingDepth.glsl", shadowPass.shader);
    // Define o framebuffer de Shadow
    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_COMPONENT, TexFilter::NEAREST, TexWrap::CLAMP_TO_BORDER, TexDType::FLOAT)};

    fbSpec.width = 2048;
    fbSpec.height = 2048;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    shadowPass.shadowBuffer = new FrameBuffer(fbSpec);
    shadowPass.lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 150.0f);
    // Note that if you use a
    // glm::mat4 lightProjection = glm::perspective(45.0f, (float)width / (float)height, near_plane, far_plane);
    // perspective projection matrix you'll have to change the light position as the
    // current light position isn't enough to reflect the whole scene.

    this->createRenderBuffer();

    // FIXME: coisa feia!!!!
    origem = new Transform();
}
Scene::~Scene() {}

void Scene::createRenderBuffer() {
    if (renderBuffer) {
        delete renderBuffer;
        renderBuffer = nullptr;
    }
    // Create RenderPass
    // Define o framebuffer de desenho
    Shader shader;
    ShaderManager::load("./assets/shaders/CanvasHMD.glsl", shader);
    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)};

    fbSpec.width = viewportWidth;
    fbSpec.height = viewportHeight;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    renderBuffer = new RenderBuffer(0, 0, new FrameBuffer(fbSpec), shader);
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

        // TODO: passar para um NativeScriptComponent
        if (entity.hasComponent<PhysicsControl>()) {
            PhysicsControl& p = entity.getComponent<PhysicsControl>();
            physicsControl = &p;
        }

        // Se for um mesh inicializar componente (já que nao tenho classe de Mesh)
        if (entity.hasComponent<MeshData>()) {
            MeshData& mesh = entity.getComponent<MeshData>();

            // Inicializa Materiais
            if (entity.hasComponent<Material>()) {
                Material& material = entity.getComponent<Material>();
                if (!material.isValid())
                    material.init();
            } else {
                Material& material = entity.addComponent<Material>();
                material.setDefaultEffect();
                material.init();
            }

            // Se nja nao foi inicializado um Renderable3dComponent ao mesh
            if (!entity.hasComponent<Renderable3dComponent>()) {

                // Transforma MeshData em VertexData comprimindo-o
                std::vector<Chimera::VertexData> renderData;
                vertexDataFromMesh(&mesh, renderData);
                std::vector<uint32_t> index;
                std::vector<Chimera::VertexData> vertexDataOut;
                vertexDataIndexCompile(renderData, vertexDataOut, index);

                // Cria o renderable object com o VertexData
                Renderable3dComponent& rc = entity.addComponent<Renderable3dComponent>();
                RenderableSimple* r = new RenderableSimple();
                r->createBuffers(&vertexDataOut[0], vertexDataOut.size(), &index[0], index.size());
                r->setEntity(entity);
                rc.renderable = r;
            }

            if (entity.hasComponent<TransComponent>()) {
                glm::vec3 min, max, size;
                vertexDataMeshMinMaxSize(&mesh, min, max, size);

                TransComponent& tc = entity.getComponent<TransComponent>();
                if (tc.solid) {
                    Solid* solid = (Solid*)tc.trans;
                    solid->init(size); // Cria rigidBody iniciaza transformacao e inicializa shape se ele nao existir
                }
            }
        }

        if (entity.hasComponent<ParticleContainer>()) {
            ParticleContainer& pc = entity.getComponent<ParticleContainer>();

            if (!entity.hasComponent<RenderableParticlesComponent>()) {

                RenderableParticlesComponent& particleSys = entity.addComponent<RenderableParticlesComponent>();
                particleSys.enable = true;
                RenderableParticles* p = new RenderableParticles();
                p->setParticleContainer(&pc);
                p->create();
                p->setEntity(entity);
                particleSys.renderable = p;
            }
        }
    });

    // initialize scripts
    registry.get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {
            nsc.instance = nsc.instantiateScript();
            nsc.instance->entity = Entity{entity, &registry};
            nsc.instance->onCreate();
        }
    });

    // load default camera
    auto view = registry.get().view<CameraComponent>();
    for (auto entity : view) {

        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (cameraComponent.primary)
            camera = cameraComponent.camera;
    }
}

void Scene::onUpdate(const double& ts) {

    // update scripts
    registry.get().view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (nsc.instance) {
            nsc.instance->onUpdate(ts);
        }
    });

    if (camera)
        camera->recalculateMatrix(false); // FIXME: remover daqui!!!!

    if (physicsControl) {
        physicsControl->stepSim(ts);
        physicsControl->checkCollisions();
    }

    for (auto emissor : emitters)
        emissor->recycleLife(ts);
}

void Scene::onViewportResize(uint32_t width, uint32_t height) {

    viewportWidth = width;
    viewportHeight = height;

    auto view = registry.get().view<CameraComponent>();
    for (auto entity : view) {

        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.fixedAspectRatio) {
            cameraComponent.camera->setViewportSize(width, height);
        }
    }

    createRenderBuffer();
}

void Scene::onRender() { this->render(renderBatch); }

bool Scene::onEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    onViewportResize(event.window.data1, event.window.data2);
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
        TransComponent& tc = renderable->getEntity().getComponent<TransComponent>(); // FIXME: group this!!!

        RenderCommand command;
        command.transform = tc.trans->translateSrc(origem->getPosition());

        Shader& sc = rc.renderable->getEntity().getComponent<Shader>();
        Material& mc = rc.renderable->getEntity().getComponent<Material>();

        command.renderable = renderable;
        command.shader = sc;
        mc.bindMaterialInformation(command.uniforms, command.vTex);

        command.uniforms.push_back(UniformVal("model", command.transform));

        rc.renderable->submit(camera, command, &renderer);
    }
}

void Scene::execShadowPass(ICamera* camera, IRenderer3d& renderer) {

    // load lights after begin (clear previos lights)
    auto lightViewEnt = registry.get().view<LightComponent>();
    for (auto entity : lightViewEnt) {
        auto& lc = lightViewEnt.get<LightComponent>(entity);
        auto& tc = registry.get().get<TransComponent>(entity); // Lento
        if (lc.global) {
            // TODO: usar o direcionm depois no segundo parametro
            glm::mat4 lightView = glm::lookAt(tc.trans->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
            shadowPass.lightSpaceMatrix = shadowPass.lightProjection * lightView;
            renderer.uQueue().push_back(UniformVal("lightSpaceMatrix", shadowPass.lightSpaceMatrix));
        }
    }

    auto group = registry.get().group<TransComponent, Renderable3dComponent>();
    for (auto entity : group) {
        auto [tc, rc] = group.get<TransComponent, Renderable3dComponent>(entity);

        RenderCommand command;
        command.transform = tc.trans->translateSrc(origem->getPosition());
        command.renderable = rc.renderable;
        command.shader = shadowPass.shader;
        command.uniforms.push_back(UniformVal("model", command.transform));
        rc.renderable->submit(camera, command, &renderer);
    }
}

void Scene::execRenderPass(ICamera* camera, IRenderer3d& renderer) {

    auto group = registry.get().group<Shader, Material, TransComponent, Renderable3dComponent>();
    for (auto entity : group) {
        auto [sc, mc, tc, rc] = group.get<Shader, Material, TransComponent, Renderable3dComponent>(entity);

        RenderCommand command;
        command.transform = tc.trans->translateSrc(origem->getPosition());
        command.renderable = rc.renderable;
        command.shader = sc;
        mc.bindMaterialInformation(command.uniforms, command.vTex);
        command.uniforms.push_back(UniformVal("model", command.transform));
        rc.renderable->submit(camera, command, &renderer);
    }
}

void Scene::render(IRenderer3d& renderer) {

    if (renderer.getLog() == true) {
        glm::vec3 pos = camera->getPosition();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    }

    { // render a shadows in framebuffer
        renderer.begin(camera);
        this->execShadowPass(camera, renderer);

        renderer.end();
        shadowPass.shadowBuffer->bind(); // we're not using the stencil buffer now

        renderer.flush();
        shadowPass.shadowBuffer->unbind();
    }

    // used by all
    renderer.uQueue().push_back(UniformVal("projection", camera->getProjectionMatrix()));
    renderer.uQueue().push_back(UniformVal("view", camera->getViewMatrix()));

    // data from shadowPass
    renderer.uQueue().push_back(UniformVal("lightSpaceMatrix", shadowPass.lightSpaceMatrix));
    renderer.uQueue().push_back(UniformVal("viewPos", camera->getPosition()));
    renderer.uQueue().push_back(UniformVal("shadows", 1));   // Ativa a sombra com 1
    renderer.uQueue().push_back(UniformVal("shadowMap", 1)); // id da textura de shadow

    shadowPass.shadowBuffer->getDepthAttachemnt()->bind(1);

    auto lightView = registry.get().view<LightComponent>();
    for (auto entity : lightView) {
        auto& lc = lightView.get<LightComponent>(entity);
        auto& tc = registry.get().get<TransComponent>(entity); // lightView.get<LightComponent>(entity);
        if (lc.global)                                         // biding light prop
            lc.light->bindLight(renderer.uQueue(), tc.trans->getMatrix());
    }

    { // Render mesh
        renderer.begin(camera);
        this->execRenderPass(camera, renderer);

        renderer.end();
        renderBuffer->bind(); // we're not using the stencil buffer now

        renderer.flush();
    }

    if (emitters.size() > 0) {
        renderParticleEmitter.begin(camera);
        this->execEmitterPass(camera, renderParticleEmitter);

        renderParticleEmitter.end();
        renderParticleEmitter.flush();
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
} // namespace Chimera