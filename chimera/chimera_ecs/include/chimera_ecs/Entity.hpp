#pragma once
// #include "Registry.hpp"
#include "ecs.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

namespace ce {

    class Entity {
        entt::entity handle{entt::null};

      public:
        Entity() = default;
        Entity(const Entity& other) = default;
        Entity(entt::entity novo) : handle(novo) {}

        static Entity Create(entt::registry* r, const std::string& name, const std::string& id) {
            entt::entity handle = r->create();

            r->emplace<TagInfo>(handle, name, id);
            return handle;
        }

        // Entity& operator=(const entt::entity& other) {
        //     this->handle = other;
        //     return *this;
        // }

        static entt::entity FindEntity(entt::registry* r, const std::string& tag, bool isName = true) {
            auto view = r->view<TagInfo>();
            for (auto ent : view) {
                TagInfo& ee = r->get<TagInfo>(ent);
                if (isName) {
                    if (ee.name == tag) {
                        return ent;
                    }
                } else {
                    if (ee.id == tag) {
                        return ent;
                    }
                }
            }

            return entt::null;
        }

        template <typename T>
        static T& FindComponent(entt::registry* r, const std::string& tag, bool isName = true) {
            auto view = r->view<T>();
            for (auto ent : view) {
                TagInfo& ee = r->get<TagInfo>(ent);
                if (isName) {
                    if (ee.name == tag) {
                        return r->get<T>(ent);
                    }
                } else {
                    if (ee.id == tag) {
                        return r->get<T>(ent);
                    }
                }
            }

            if (isName) {
                throw std::invalid_argument(std::string("name not found: ") + tag);
            }

            throw std::invalid_argument(std::string("id not found: ") + tag);
        }

        void destroy(entt::registry* r) {
            r->destroy(handle);
            handle = entt::null;
        }

        template <typename T>
        bool hasComponent(entt::registry* r) const {
            return r->all_of<T>(handle);
        }

        template <typename T, typename... Args>
        T& addComponent(entt::registry* r, Args&&... args) {
            return r->emplace<T>(handle, std::forward<Args>(args)...);
        }

        template <typename T>
        T& getComponent(entt::registry* r) {
            return r->get<T>(handle);
        }

        template <typename T>
        void removeComponent(entt::registry* r) {
            r->remove<T>(handle);
        }

        operator bool() const { return handle != entt::null; }

        operator uint32_t() const { return (uint32_t)handle; }

        operator entt::entity() const { return handle; }

        bool operator==(const Entity& other) const { return (handle == other.handle); }

        bool operator!=(const Entity& other) const { return !(*this == other); }
    };
} // namespace ce
