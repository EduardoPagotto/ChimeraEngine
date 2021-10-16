#ifndef __CHIMERA_ENTITY_ZZ__HPP
#define __CHIMERA_ENTITY_ZZ__HPP

#include "Entity.hpp"
#include "Scene.hpp"
#include <entt/entt.hpp>

namespace Chimera {
class Scene;
class Entity {
  public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene) : entityHandle(handle), scene(scene) {}
    Entity(const Entity& other) = default;

    template <typename T> bool hasComponent() { return scene->eRegistry.all_of<T>(entityHandle); }
    template <typename T, typename... Args> T& addComponent(Args&&... args) {
        return scene->eRegistry.emplace<T>(entityHandle, std::forward<Args>(args)...);
    }
    template <typename T> T& getComponent() { return scene->eRegistry.get<T>(entityHandle); }
    template <typename T> void removeComponent() { scene->eRegistry.remove<T>(entityHandle); }
    operator bool() const { return entityHandle != entt::null; }

    operator uint32_t() const { return (uint32_t)entityHandle; }
    operator entt::entity() const { return entityHandle; }
    bool operator==(const Entity& other) const { return entityHandle == other.entityHandle && scene == other.scene; }
    bool operator!=(const Entity& other) const { return !(*this == other); }

  private:
    entt::entity entityHandle{entt::null};
    Scene* scene = nullptr;
};
} // namespace Chimera
#endif