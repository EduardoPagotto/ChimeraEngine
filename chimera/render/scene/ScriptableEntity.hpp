#ifndef __CHIMERA_SCENE_SCRIPTABLE_ENTITY__HPP
#define __CHIMERA_SCENE_SCRIPTABLE_ENTITY__HPP

#include "Entity.hpp"

namespace Chimera {

class ScriptableEntity {
  public:
    virtual ~ScriptableEntity(){};

    template <typename T> T& getComponent() { return entity.getComponent<T>(); }

    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float ts) {}

    void setEntity(const Entity& entity) { this->entity = entity; }

  private:
    Entity entity;
    friend class Scene;
};

} // namespace Chimera

#endif