#ifndef __CHIMERA_SCENE_SCRIPTABLE_ENTITY__HPP
#define __CHIMERA_SCENE_SCRIPTABLE_ENTITY__HPP

#include "Entity.hpp"

namespace Chimera {

class ScriptableEntity {
  public:
    virtual ~ScriptableEntity(){};

    template <typename T> T& getComponent() { return entity.getComponent<T>(); }

  protected:
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float ts) {}

  private:
    Entity entity;
    friend class Scene;
};

} // namespace Chimera

#endif