#pragma once
#include "TagComponent.hpp"
#include <entt/entt.hpp>
#include <stdexcept>

namespace Chimera {

class Entity;
class RegistryManager;

class Registry {
  public:
    virtual ~Registry() { destroy(); }
    void destroy() { eRegistry.clear(); }
    Entity createEntity(const std::string& tag = std::string(), const std::string& id = std::string());
    void destroyEntity(Entity entity);
    entt::registry& get() { return eRegistry; }

    template <typename T>
    T& findComponent(const std::string& tagName) {
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
    friend RegistryManager;
    Registry() = default;
    Registry(const Registry& other) = delete;
    Registry& operator=(const Registry& other) = delete;
    entt::registry eRegistry;
    inline static uint32_t masterSerial = 0;
};

class RegistryManager {
  public:
    static size_t create() {
        r.push_back(new Registry());
        return r.size();
    }

    static void destroy(const size_t& index) {
        auto i = r.begin() + index;
        (*i)->destroy();
        r.erase(i);
    }

    static Registry& get(const size_t& index = 0) { return *r[index]; }
    static Registry* getPtr(const size_t& index = 0) { return r[index]; }

  private:
    RegistryManager() = delete;
    RegistryManager(const RegistryManager& other) = delete;
    RegistryManager& operator=(const RegistryManager& other) = delete;

    inline static std::vector<Registry*> r;
};

class Entity {
  public:
    Entity() = default;
    Entity(entt::entity handle, Registry* reg) : handle(handle), reg(reg) {}
    Entity(const Entity& other) : handle(other.handle), reg(other.reg) {}
    template <typename T>
    const bool hasComponent() const {
        return reg->eRegistry.all_of<T>(handle);
    }
    template <typename T, typename... Args>
    T& addComponent(Args&&... args) {
        return reg->eRegistry.emplace<T>(handle, std::forward<Args>(args)...);
    }
    template <typename T>
    T& getComponent() {
        return reg->eRegistry.get<T>(handle);
    }
    template <typename T>
    void removeComponent() {
        reg->eRegistry.remove<T>(handle);
    }
    operator bool() const { return handle != entt::null; }
    operator uint32_t() const { return (uint32_t)handle; }
    operator entt::entity() const { return handle; }
    bool operator==(const Entity& other) const { return handle == other.handle && reg == other.reg; }
    bool operator!=(const Entity& other) const { return !(*this == other); }
    Registry* getRegistry() const { return reg; }

  private:
    entt::entity handle{entt::null};
    Registry* reg = nullptr;
};

// struct Hierarchie {
//     Hierarchie() = default;
//     std::size_t children{};
//     Entity local, first, prev, next, parent;
// };

// class Relationship {
//   public:
//     Relationship(Hierarchie* h) : hierarchie(h) {}
//     Relationship(const Relationship& o) : hierarchie(o.hierarchie) {}
//     Relationship(Registry& r, const std::string& tag, const std::string& id) {
//         Entity e = r.createEntity(tag, id);
//         Relationship& novo = e.addComponent<Relationship>(new Hierarchie());
//         novo.get()->local = e;
//         this->hierarchie = novo.get();
//     }
//     Hierarchie* get() const { return hierarchie; }

//     Relationship addChild(const std::string& tag, const std::string& id) {
//         Relationship novo(*hierarchie->local.getRegistry(), tag, id);
//         novo.get()->parent = hierarchie->local;

//         if (hierarchie->children == 0) {
//             hierarchie->first = novo.get()->local;
//         } else {
//             Relationship& last = hierarchie->first.getComponent<Relationship>();
//             while (last.get()->next != Entity())
//                 last = last.get()->next.getComponent<Relationship>();

//             last.get()->next = novo.get()->local;
//             novo.get()->prev = last.get()->local;
//         }

//         hierarchie->children++;
//         return novo;
//     }

//   private:
//     Hierarchie* hierarchie = nullptr;
// };
// ref: https://skypjack.github.io/2019-06-25-ecs-baf-part-4/
// Engine engine;
// Relationship obj0(engine.getRegistry(), "root", "root");
// Relationship child0 = obj0.addChild("child00", "child00");
// Relationship child1 = obj0.addChild("child01", "child01");
// Relationship child2 = obj0.addChild("child02", "child02");
// Relationship child3 = child2.addChild("child03", "child03");
// Relationship child4 = child2.addChild("child04", "child04");
} // namespace Chimera