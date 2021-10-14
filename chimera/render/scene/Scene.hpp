#ifndef __CHIMERA_SCENE_ZZ1__HPP
#define __CHIMERA_SCENE_ZZ1__HPP

#include <entt/entt.hpp>

namespace Chimera {

class Entity;
class Scene {
  public:
    Scene();
    virtual ~Scene();

    Entity createEntity(const std::string& name = std::string());
    void onUpdate(float ts);

  private:
    entt::registry eRegistry;

    friend class Entity;
};

} // namespace Chimera

#endif