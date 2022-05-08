#pragma once
#include "TagComponent.hpp"
#include <entt/entt.hpp>
#include <stdexcept>

namespace Chimera {

class Entity;
class Registry {
  public:
    Registry() = default;
    Entity createEntity(const std::string& tag = std::string(), const std::string& id = std::string());
    void destroyEntity(Entity entity);
    entt::registry& get() { return eRegistry; }

    Entity findEntityTag(const std::string& tag);
    Entity findEntityId(const std::string& id);
    Entity findEntitySerial(const uint32_t& serial);

    template <typename T> T& findComponent(const std::string& tagName) {
        auto view = eRegistry.view<T>();
        for (auto ent : view) {
            TagComponent& ee = eRegistry.get<TagComponent>(ent);
            if (ee.tag == tagName)
                return eRegistry.get<T>(ent);
        }
        throw std::invalid_argument(std::string("Tag name nao encontrado: ") + tagName);
    }

  private:
    friend Entity;
    entt::registry eRegistry;
    static uint32_t masterSerial;
};

class Entity {
  public:
    Entity() = default;
    Entity(entt::entity handle, Registry* reg) : handle(handle), reg(reg) {}
    Entity(const Entity& other) : handle(other.handle), reg(other.reg) {}
    template <typename T> bool hasComponent() { return reg->eRegistry.all_of<T>(handle); }
    template <typename T, typename... Args> T& addComponent(Args&&... args) {
        return reg->eRegistry.emplace<T>(handle, std::forward<Args>(args)...);
    }
    template <typename T> T& getComponent() { return reg->eRegistry.get<T>(handle); }
    template <typename T> void removeComponent() { reg->eRegistry.remove<T>(handle); }
    operator bool() const { return handle != entt::null; }
    operator uint32_t() const { return (uint32_t)handle; }
    operator entt::entity() const { return handle; }
    bool operator==(const Entity& other) const { return handle == other.handle && reg == other.reg; }
    bool operator!=(const Entity& other) const { return !(*this == other); }

  private:
    entt::entity handle{entt::null};
    Registry* reg = nullptr;
};
} // namespace Chimera