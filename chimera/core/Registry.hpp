// #pragma once
// #include <entt/entt.hpp>

// namespace Chimera {

// struct TagComponent {
//     std::string tag;
//     std::string id;
//     TagComponent() = default;
//     TagComponent(const std::string& tag, const std::string& id) : tag(tag), id(id) {}
// };

// class Entity;
// class Registry {
//   public:
//     static Entity CreateEntity(const std::string& name = std::string());
//     static void DestroyEntity(Entity entity);
//     static entt::registry& GetRegistry() { return eRegistry; }

//   protected:
//     Registry() = default;

//   private:
//     static entt::registry eRegistry;
// };

// class Entity {
//   public:
//     Entity() = default;
//     Entity(entt::entity handle) : handle(handle) {}
//     Entity(const Entity& other) { handle = other.handle; }

//     template <typename T> bool hasComponent() { return Registry::GetRegistry().all_of<T>(handle); }
//     template <typename T, typename... Args> T& addComponent(Args&&... args) {
//         return Registry::GetRegistry().emplace<T>(handle, std::forward<Args>(args)...);
//     }
//     template <typename T> T& getComponent() { return Registry::GetRegistry().get<T>(handle); }
//     template <typename T> void removeComponent() { Registry::GetRegistry().remove<T>(handle); }
//     operator bool() const { return handle != entt::null; }

//     operator uint32_t() const { return (uint32_t)handle; }
//     operator entt::entity() const { return handle; }
//     bool operator==(const Entity& other) const { return handle == other.handle; }
//     bool operator!=(const Entity& other) const { return !(*this == other); }

//   private:
//     entt::entity handle{entt::null};
// };
// } // namespace Chimera