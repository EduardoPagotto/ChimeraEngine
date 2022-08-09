#pragma once
#include "chimera/core/Registry.hpp"

namespace Chimera {

class ScriptableEntity {
  public:
    virtual ~ScriptableEntity(){};

    template <typename T> T& getComponent() { return entity.getComponent<T>(); }

    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(const double& ts) {}

    void setEntity(const Entity& entity) { this->entity = entity; }

  private:
    Entity entity;
    friend class Scene;
};

struct NativeScriptComponent {

    std::string name;
    ScriptableEntity* instance = nullptr;

    ScriptableEntity* (*instantiateScript)();
    void (*destroyScript)(NativeScriptComponent*);

    template <typename T> void bind(const std::string& nameBind) {
        name = nameBind;
        instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
        destroyScript = [](NativeScriptComponent* nsc) {
            delete nsc->instance;
            nsc->instance = nullptr;
        };
    }
};

} // namespace Chimera
