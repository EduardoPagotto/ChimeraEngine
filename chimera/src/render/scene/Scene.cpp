#include "chimera/render/scene/Scene.hpp"
#include "chimera/core/MouseDevice.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/3d/RenderableSimple.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/Transform.hpp"
#include "chimera/render/buffer/VertexArray.hpp"
#include "chimera/render/bullet/Solid.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"
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
    emissor = nullptr;
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
    eRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {
            nsc.instance->onDestroy();
            nsc.destroyScript(&nsc);
        }
    });
}

void Scene::onAttach() {
    // lista as tags nas entidades registradas
    eRegistry.each([&](auto entityID) {
        Entity entity{entityID, this};
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

            if (entity.hasComponent<Solid>()) {
                glm::vec3 min, max, size;
                vertexDataMeshMinMaxSize(&mesh, min, max, size);

                Solid& solid = entity.getComponent<Solid>();
                solid.init(size); // Cria rigidBody iniciaza transformacao e inicializa shape se ele nao existir
            }
        }

        // TODO: melhorar!!!!
        if (entity.hasComponent<EmiterComponent>()) {
            EmiterComponent& ec = entity.getComponent<EmiterComponent>();
            emissor = ec.emitter;
        }
    });

    // initialize scripts
    eRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {
            nsc.instance = nsc.instantiateScript();
            nsc.instance->entity = Entity{entity, this};
            nsc.instance->onCreate();
        }
    });

    // load default camera
    auto view = eRegistry.view<CameraComponent>();
    for (auto entity : view) {

        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (cameraComponent.primary)
            camera = cameraComponent.camera;
    }
}

void Scene::onUpdate(const double& ts) {

    // update scripts
    eRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
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

    // TODO: melhorar!!!!
    if (emissor) {
        emissor->recycleLife(ts); // TODO: passar para array e melhorar onde guardar o relacionamento!!
    }
}

Entity Scene::createEntity(const std::string& name) {

    // toda entidade tem um transform
    Entity entity = {eRegistry.create(), this};
    entity.addComponent<Transform>();
    auto& tag = entity.addComponent<TagComponent>();
    tag.tag = name.empty() ? "Entity" : name;

    return entity;
}

void Scene::destroyEntity(Entity entity) { eRegistry.destroy(entity); }

void Scene::onViewportResize(uint32_t width, uint32_t height) {

    viewportWidth = width;
    viewportHeight = height;

    auto view = eRegistry.view<CameraComponent>();
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

    auto view = eRegistry.view<RenderableParticle>();
    for (auto entity : view) {

        RenderableParticle& rc = view.get<RenderableParticle>(entity);
        IRenderable3d* renderable = rc.renderable;

        RenderCommand command;

        if (renderable->getEntity().hasComponent<Solid>()) {
            Solid& sl = renderable->getEntity().getComponent<Solid>();
            command.transform = sl.getMatrix();
        } else {
            Transform& tc = renderable->getEntity().getComponent<Transform>();
            command.transform = tc.getMatrix();
        }

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
    auto lightViewEnt = eRegistry.view<LightComponent>();
    for (auto entity : lightViewEnt) {
        auto& lc = lightViewEnt.get<LightComponent>(entity);
        if (lc.global) {
            // TODO: usar o direcionm depois no segundo parametro
            glm::mat4 lightView = glm::lookAt(lc.light->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 0.0, -1.0));
            shadowPass.lightSpaceMatrix = shadowPass.lightProjection * lightView;
            renderer.submitUniform(UniformVal("lightSpaceMatrix", shadowPass.lightSpaceMatrix));
        }
    }

    auto view = eRegistry.view<Renderable3dComponent>();
    for (auto entity : view) {

        Renderable3dComponent& rc = view.get<Renderable3dComponent>(entity);
        IRenderable3d* renderable = rc.renderable;

        RenderCommand command;

        if (renderable->getEntity().hasComponent<Solid>()) {
            Solid& sl = renderable->getEntity().getComponent<Solid>();
            command.transform = sl.getMatrix();
        } else {
            Transform& tc = renderable->getEntity().getComponent<Transform>();
            command.transform = tc.getMatrix();
        }

        command.renderable = renderable;
        command.shader = shadowPass.shader;
        command.uniforms.push_back(UniformVal("model", command.transform));
        rc.renderable->submit(camera, command, &renderer);
    }
}

void Scene::execRenderPass(ICamera* camera, IRenderer3d& renderer) {
    auto view = eRegistry.view<Renderable3dComponent>();
    for (auto entity : view) {

        Renderable3dComponent& rc = view.get<Renderable3dComponent>(entity);
        IRenderable3d* renderable = rc.renderable;

        RenderCommand command;

        if (renderable->getEntity().hasComponent<Solid>()) {
            Solid& sl = renderable->getEntity().getComponent<Solid>();
            command.transform = sl.getMatrix();
        } else {
            Transform& tc = renderable->getEntity().getComponent<Transform>();
            command.transform = tc.getMatrix();
        }

        Shader& sc = rc.renderable->getEntity().getComponent<Shader>();
        Material& mc = rc.renderable->getEntity().getComponent<Material>();

        command.renderable = renderable;
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
    renderer.submitUniform(UniformVal("projection", camera->getProjectionMatrix()));
    renderer.submitUniform(UniformVal("view", camera->getViewMatrix()));

    // data from shadowPass
    renderer.submitUniform(UniformVal("lightSpaceMatrix", shadowPass.lightSpaceMatrix));
    renderer.submitUniform(UniformVal("viewPos", camera->getPosition()));
    renderer.submitUniform(UniformVal("shadows", 1));   // Ativa a sombra com 1
    renderer.submitUniform(UniformVal("shadowMap", 1)); // id da textura de shadow

    shadowPass.shadowBuffer->getDepthAttachemnt()->bind(1);

    auto lightView = eRegistry.view<LightComponent>();
    for (auto entity : lightView) {
        auto& lc = lightView.get<LightComponent>(entity);
        if (lc.global) {
            renderer.submitLight(lc.light);
        }
    }

    { // Render mesh
        renderer.begin(camera);
        this->execRenderPass(camera, renderer);

        renderer.end();
        renderBuffer->bind(); // we're not using the stencil buffer now

        renderer.flush();
    }

    if (emissor) { // Render emissores
        renderParticleEmitter.begin(camera);
        this->execEmitterPass(camera, renderParticleEmitter);

        renderParticleEmitter.end();
        renderParticleEmitter.flush();
    }

    {
        // get val from color buffer (must be inside framebuffer renderer
        glm::ivec2 pos = MouseDevice::getMove();
        pos.y = viewportHeight - pos.y;
        int val = renderBuffer->getFramBuffer()->readPixel(1, pos.x, pos.y);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "mouse(X: %d / Y: %d): %d", pos.x, pos.y, val);
    }

    renderBuffer->unbind();
    renderBuffer->render();
}
} // namespace Chimera