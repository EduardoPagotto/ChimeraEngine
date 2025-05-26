#pragma once
#include "Registry.hpp"

namespace ce {

    class Entity {
        entt::entity handle{entt::null};

      public:
        Entity() = default;

        Entity(const Entity& other) : handle(other.handle) {}

        Entity(const std::string& name, const std::string& id) {
            handle = g_registry.get().create();
            this->addComponent<TagInfo>(TagInfo{name, id});
        }

        Entity(entt::entity novo) : handle(novo) {}

        // Entity& operator=(const entt::entity& other) {
        //     this->handle = other;
        //     return *this;
        // }

        void destroy() {
            { g_registry.get().destroy(handle); }
            handle = entt::null;
        }

        template <typename T>
        const bool hasComponent() const {
            return g_registry.get().all_of<T>(handle);
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args) {
            return g_registry.get().emplace<T>(handle, std::forward<Args>(args)...);
        }

        template <typename T>
        T& getComponent() {
            return g_registry.get().get<T>(handle);
        }

        template <typename T>
        void removeComponent() {
            g_registry.get().remove<T>(handle);
        }

        operator bool() const { return handle != entt::null; }

        operator uint32_t() const { return (uint32_t)handle; }

        operator entt::entity() const { return handle; }

        bool operator==(const Entity& other) const { return (handle == other.handle); }

        bool operator!=(const Entity& other) const { return !(*this == other); }
    };
} // namespace ce
