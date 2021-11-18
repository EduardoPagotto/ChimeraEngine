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

Scene::Scene() : camera(nullptr), viewportWidth(800), viewportHeight(640), renderPass(nullptr), physicsControl(nullptr) {
    renderPassShadow = new RenderPassShadow(2048, 2048);

    emissor = nullptr;
}
Scene::~Scene() {}

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

    if (renderPass) {
        delete renderPass;
        renderPass = nullptr;
    }

    renderPass = new RenderPass(width, height);
}

void Scene::onRender() {
    this->render(renderBatch);

    // renderParticleEmitter.begin(camera);
    //---------

    // TODO: melhorar!!!!
    renderParticleEmitter.begin(camera);

    auto view = eRegistry.view<RenderableParticle>();
    for (auto entity : view) {

        RenderableParticle& rc = view.get<RenderableParticle>(entity);
        IRenderable3d* renderable = rc.renderable;

        RenderCommand command;

        // if (renderable->getEntity().hasComponent<Solid>()) {
        //     Solid& sl = renderable->getEntity().getComponent<Solid>();
        //     command.transform = sl.getMatrix();
        // } else {
        Transform& tc = renderable->getEntity().getComponent<Transform>();
        command.transform = tc.getMatrix();
        // }

        Shader& sc = rc.renderable->getEntity().getComponent<Shader>();
        Material& mc = rc.renderable->getEntity().getComponent<Material>();

        command.renderable = renderable;
        command.shader = sc;
        mc.bindMaterialInformation(command.uniforms, command.vTex);

        command.uniforms.push_back(UniformVal("model", command.transform));

        rc.renderable->submit(camera, command, &renderParticleEmitter);
    }

    renderParticleEmitter.end();

    // renderBuffer->bind(); // we're not using the stencil buffer now

    renderParticleEmitter.flush();
}

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

void Scene::render(IRenderer3d& renderer) {

    if (renderer.getLog() == true) {
        glm::vec3 pos = camera->getPosition();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    }

    // render a shadows in framebuffer
    renderPassShadow->execute(camera, renderer, eRegistry);

    // used by all
    renderer.submitUniform(UniformVal("projection", camera->getProjectionMatrix()));
    renderer.submitUniform(UniformVal("view", camera->getViewMatrix()));

    // udes by shadows
    renderer.submitUniform(UniformVal("lightSpaceMatrix", renderPassShadow->getLightSpaceMatrix()));
    renderer.submitUniform(UniformVal("viewPos", camera->getPosition()));
    renderer.submitUniform(UniformVal("shadows", 1));   // Ativa a sombra com 1
    renderer.submitUniform(UniformVal("shadowMap", 1)); // id da textura de shadow
    renderPassShadow->bindDepthBuffer();

    auto lightView = eRegistry.view<LightComponent>();
    for (auto entity : lightView) {
        auto& lc = lightView.get<LightComponent>(entity);
        if (lc.global) {
            renderer.submitLight(lc.light);
        }
    }

    renderPass->execute(camera, renderer, eRegistry);

    renderPass->render();
}
} // namespace Chimera