#include "chimera/render/scene/Scene.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

Scene::Scene() {}
Scene::~Scene() {}

void Scene::onUpdate(float ts) {

    // update scripts

    eRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
        if (!nsc.instance) {

            nsc.instance = nsc.instantiateScript();
            nsc.instance->entity = Entity{entity, this}; // set de enity to native script
            nsc.instance->onCreate();
        }

        nsc.instance->onUpdate(ts);
    });

    Camera* mainCamera = nullptr;
    glm::mat4* cameraTransform = nullptr;

    auto group1 = eRegistry.group<TransformComponent, CameraComponent>();
    for (auto entity : group1) {

        auto [transform, camera] = group1.get<TransformComponent, CameraComponent>(entity);
        if (camera.primary) {
            mainCamera = &camera.camera;
            cameraTransform = &transform.transform;
            break;
        }
    }

    if (mainCamera) {

        // proximo scene camera  https://www.youtube.com/watch?v=UKVFRRufKzo

        // Renderer2D::begin(*mainCamera, *cameraTransform); // inver transform depois

        // auto group = eRegistry.group<TransformComponent>(entt::get<SpriteComponent>);
        // for (auto entity : group) {

        //     auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
        //     // Renderer2D::DrawQuad(transform, sprite);
        // }
    }
}

Entity Scene::createEntity(const std::string& name) {

    // toda entidade tem um transform
    Entity entity = {eRegistry.create(), this};
    entity.addComponent<TransformComponent>();
    auto& tag = entity.addComponent<TagComponent>();
    tag.tag = name.empty() ? "Entity" : name;

    return entity;
}

void Scene::onViewportResize(uint32_t width, uint32_t height) {

    viewportWidth = width;
    viewportHeight = height;

    auto view = eRegistry.view<CameraComponent>();
    for (auto entity : view) {

        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.fixedAspectRatio) {

            cameraComponent.camera.setViewportSize(width, height);
        }
    }
}

} // namespace Chimera