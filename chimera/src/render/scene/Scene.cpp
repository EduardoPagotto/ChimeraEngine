#include "chimera/render/scene/Scene.hpp"
#include "chimera/core/Transform.hpp"
#include "chimera/core/buffers/VertexArray.hpp"
#include "chimera/render/3d/RenderCommand.hpp"
#include "chimera/render/Material.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

Scene::Scene() : camera(nullptr), viewportWidth(800), viewportHeight(640), renderBuffer(nullptr) {}
Scene::~Scene() {}

void Scene::onDestroy() {
    // destroy scripts
    eRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {
            nsc.instance->onDestroy();
            nsc.destroyScript(&nsc);
        }
    });
}

void Scene::onCreate() {
    // lista as tags nas entidades registradas
    eRegistry.each([&](auto entityID) {
        Entity entity{entityID, this};
        auto& tc = entity.getComponent<TagComponent>();
        SDL_Log("Tag: %s", tc.tag.c_str());
    });

    // initialize scripts
    eRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {
            nsc.instance = nsc.instantiateScript();
            nsc.instance->entity = Entity{entity, this};
            nsc.instance->onCreate();
        }
    });
}

void Scene::onUpdate(float ts) {

    // update scripts
    eRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (nsc.instance) {
            nsc.instance->onUpdate(ts);
        }
    });

    camera->recalculateMatrix(false);
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
            cameraComponent.camera->setAspectRatio(width, height);
        }
    }

    if (renderBuffer) {
        delete renderBuffer;
        renderBuffer = nullptr;
    }

    {
        // TODO: Testar
        FrameBufferSpecification fbSpec;
        fbSpec.attachments = {
            TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE),
            TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE),
            TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)};

        fbSpec.width = width;
        fbSpec.height = height;
        fbSpec.swapChainTarget = false;
        fbSpec.samples = 1;

        FrameBufferZ* fbz = new FrameBufferZ(fbSpec);
    }

    Shader shader;
    ShaderManager::load("./assets/shaders/CanvasHMD.glsl", shader);
    renderBuffer = new RenderBuffer(0, 0, width, height, shader);
}

void Scene::render(IRenderer3d& renderer) {

    this->onUpdate(0.01); // atualiza camera e script de camera

    if (renderer.getLog() == true) {
        glm::vec3 pos = camera->getPosition();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    }

    renderBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now

    renderer.begin(camera);

    for (Light* l : lightSetupStack) {
        renderer.submitLight(l);
    }

    auto view = eRegistry.view<Renderable3dComponent>();
    for (auto entity : view) {

        Renderable3dComponent& rc = view.get<Renderable3dComponent>(entity);
        IRenderable3d* renderable = rc.renderable;

        Transform& tc = renderable->getEntity().getComponent<Transform>();
        Shader& sc = rc.renderable->getEntity().getComponent<Shader>();
        Material& mc = rc.renderable->getEntity().getComponent<Material>();

        RenderCommand command;
        command.renderable = renderable;
        command.transform = tc.getMatrix();
        command.shader = sc;
        mc.bindMaterialInformation(command.uniforms, command.vTex);

        // FIXME: preciso disto aqui ??
        command.uniforms.push_back(UniformVal("shadows", (int)0));

        rc.renderable->submit(camera, command, &renderer);
    }

    renderer.end();
    renderer.flush();

    renderBuffer->unbind();

    // VertexArray::unbind();
    // VertexBuffer::unbind();
    // Shader::disable();

    renderBuffer->renderText();

    // this->onUpdate(0.01); // atualiza camera e script de camera

    // if (renderer.getLog() == true) {
    //     glm::vec3 pos = camera->getPosition();
    //     SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    // }

    // // camera->recalculateMatrix(canvas->getRatio());// ainda nao sei o que fazer aqui

    // renderer.begin(camera);

    // for (Light* l : lightSetupStack) {
    //     renderer.submitLight(l);
    // }

    // auto group1 = eRegistry.group<Renderable3dComponent, Transform, Shader, Material>();
    // for (auto entity : group1) {
    //     auto [rc, tc, sc, mc] = group1.get<Renderable3dComponent, Transform, Shader, Material>(entity);

    //     IRenderable3d* renderable = rc.renderable;
    //     RenderCommand command;
    //     command.renderable = renderable;
    //     command.transform = tc.getMatrix();
    //     command.shader = sc;
    //     mc.bindMaterialInformation(command.uniforms);

    //     // FIXME: preciso disto aqui ??
    //     command.uniforms.push_back(UniformVal("shadows", (int)0));

    //     rc.renderable->submit(camera, command, &renderer);
    // }

    // renderer.end();
    // renderer.flush();
}

} // namespace Chimera