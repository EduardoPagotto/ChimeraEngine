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

Scene::Scene() : camera(nullptr), viewportWidth(800), viewportHeight(640), renderBuffer(nullptr), physicsControl(nullptr) {}
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

    Shader shader;
    ShaderManager::load("./assets/shaders/CanvasHMD.glsl", shader);

    FrameBufferSpecification fbSpec;
    fbSpec.attachments = {
        TexParam(TexFormat::RGBA, TexFormat::RGBA, TexFilter::LINEAR, TexWrap::CLAMP, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::RED_INTEGER, TexFormat::R32I, TexFilter::LINEAR, TexWrap::CLAMP_TO_EDGE, TexDType::UNSIGNED_BYTE),
        TexParam(TexFormat::DEPTH_COMPONENT, TexFormat::DEPTH_ATTACHMENT, TexFilter::NONE, TexWrap::NONE, TexDType::UNSIGNED_BYTE)};

    fbSpec.width = width;
    fbSpec.height = height;
    fbSpec.swapChainTarget = false;
    fbSpec.samples = 1;

    if (renderBuffer) {
        delete renderBuffer;
        renderBuffer = nullptr;
    }

    renderBuffer = new RenderBuffer(0, 0, new FrameBuffer(fbSpec), shader);
}

void Scene::render(IRenderer3d& renderer) {

    if (renderer.getLog() == true) {
        glm::vec3 pos = camera->getPosition();
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Eye: %0.2f; %0.3f; %0.3f", pos.x, pos.y, pos.z);
    }

    renderBuffer->bind(); // we're not using the stencil buffer now
    renderer.begin(camera);

    // load lights after begin (clear previos lights)
    auto lightView = eRegistry.view<LightComponent>();
    for (auto entity : lightView) {
        auto& lc = lightView.get<LightComponent>(entity);
        if (lc.global) {
            renderer.submitLight(lc.light);
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

        // Transform& tc = renderable->getEntity().getComponent<Transform>();
        Shader& sc = rc.renderable->getEntity().getComponent<Shader>();
        Material& mc = rc.renderable->getEntity().getComponent<Material>();

        command.renderable = renderable;
        // command.transform = tc.getMatrix();
        command.shader = sc;
        mc.bindMaterialInformation(command.uniforms, command.vTex);

        // FIXME: preciso disto aqui ??
        command.uniforms.push_back(UniformVal("shadows", (int)0));

        rc.renderable->submit(camera, command, &renderer);
    }

    renderer.end();
    renderer.flush();

    // get val from color buffer (must be inside framebuffer renderer
    glm::ivec2 pos = MouseDevice::getMove();
    pos.y = viewportHeight - pos.y;
    int val = fb->readPixel(1, pos.x, pos.y);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "mouse(X: %d / Y: %d): %d", pos.x, pos.y, val);

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