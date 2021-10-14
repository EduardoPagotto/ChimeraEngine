#include "chimera/render/scene/Scene.hpp"
#include "chimera/render/scene/Components.hpp"
#include "chimera/render/scene/Entity.hpp"

namespace Chimera {

static void doMath(const glm::mat4& transform) {}

Scene::Scene() {

    // struct Mesh {
    //     float test;
    //     Mesh() = default;
    // };
    // struct TransformComponent {
    //     glm::mat4 transform;

    //     TransformComponent() = default;
    //     TransformComponent(const TransformComponent& transform) = default;
    //     TransformComponent(glm::mat4 transform) : transform(transform) {}

    //     operator glm::mat4() { return transform; }
    //     operator const glm::mat4() const { return transform; }
    // };

    // TransformComponent transform;
    // doMath(transform);

    // entt::entity entity = eRegistry.create();
    // bool vv = eRegistry.all_of<TransformComponent>(entity);

    // auto& trans = eRegistry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

    // auto view = eRegistry.view<TransformComponent>();

    // for (auto entity : view) {

    //     TransformComponent tr = eRegistry.get<TransformComponent>(entity);
    // }

    // auto group = eRegistry.group<TransformComponent>(entt::get<Mesh>);
    // for (auto entity : group) {

    //     auto& [transform, mesh] = group.get<TransformComponent, Mesh>(entity);
    //     // renderer::subimit(mesh, transform);
    // }
}
Scene::~Scene() {}

void Scene::onUpdate(float ts) {
    auto group = eRegistry.group<TransformComponent>(entt::get<SpriteComponent>);
    for (auto entity : group) {

        auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
    }
}

// Entity createEntity() { return Entity(eRegistry.create(), this); } // return {eRegistry.create(), this}; }
// entt::entity Scene::createEntity() { return eRegistry.create(); }

Entity Scene::createEntity(const std::string& name) {

    // toda entidade tem um transform
    Entity entity = {eRegistry.create(), this};
    entity.addComponent<TransformComponent>();
    auto& tag = entity.addComponent<TagComponent>();
    tag.tag = name.empty() ? "Entity" : name;

    return entity; // 26:53 ref: https://www.youtube.com/watch?v=GfSzeAcsBb0
}

} // namespace Chimera