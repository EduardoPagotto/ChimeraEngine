#ifndef __CHIMERA_SCENE_SCRIPTABLE_ENTITY__HPP
#define __CHIMERA_SCENE_SCRIPTABLE_ENTITY__HPP

#include "Entity.hpp"

namespace Chimera {

class ScriptableEntity {
  public:
    template <typename T> T& getComponent() { return entity.getComponent<T>(); }

  private:
    Entity entity;
    friend class Scene;
};

} // namespace Chimera

#endif