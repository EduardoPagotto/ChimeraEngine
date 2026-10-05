#pragma once
// #include "Registry.hpp"
#include "ecs.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

namespace ce {

    class Entity {
      public:
        Entity() = default;
        Entity(const Entity& other) = default;
        Entity(entt::entity novo) : handle_(novo) {}

        static Entity create(entt::registry* r, const std::string& name, const std::string& id) {
            entt::entity handle = r->create();

            r->emplace<TagInfo>(handle, name, id);
            return handle;
        }

        // Entity& operator=(const entt::entity& other) {
        //     this->handle = other;
        //     return *this;
        // }

        static entt::entity find_entity(entt::registry* r, const std::string& tag, bool is_name = true) {
            auto view = r->view<TagInfo>();
            for (auto ent : view) {
                TagInfo& ee = r->get<TagInfo>(ent);
                if (is_name) {
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
        static T& find_component(entt::registry* r, const std::string& tag, bool is_name = true) {
            auto view = r->view<T>();
            for (auto ent : view) {
                TagInfo& ee = r->get<TagInfo>(ent);
                if (is_name) {
                    if (ee.name == tag) {
                        return r->get<T>(ent);
                    }
                } else {
                    if (ee.id == tag) {
                        return r->get<T>(ent);
                    }
                }
            }

            if (is_name) {
                throw std::invalid_argument(std::string("name not found: ") + tag);
            }

            throw std::invalid_argument(std::string("id not found: ") + tag);
        }

        void destroy(entt::registry* r) {
            r->destroy(handle_);
            handle_ = entt::null;
        }

        template <typename T>
        bool has_component(entt::registry* r) const {
            return r->all_of<T>(handle_);
        }

        template <typename T, typename... Args>
        T& add_component(entt::registry* r, Args&&... args) {
            return r->emplace<T>(handle_, std::forward<Args>(args)...);
        }

        template <typename T>
        T& get_component(entt::registry* r) {
            return r->get<T>(handle_);
        }

        template <typename T>
        void remove_component(entt::registry* r) {
            r->remove<T>(handle_);
        }

        operator bool() const { return handle_ != entt::null; }

        operator uint32_t() const { return (uint32_t)handle_; }

        operator entt::entity() const { return handle_; }

        bool operator==(const Entity& other) const { return (handle_ == other.handle_); }

        bool operator!=(const Entity& other) const { return !(*this == other); }

      private:
        entt::entity handle_{entt::null};
    };
} // namespace ce
