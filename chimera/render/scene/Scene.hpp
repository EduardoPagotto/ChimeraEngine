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
    void destroyEntity(Entity entity);
    void onUpdate(float ts);
    void onViewportResize(uint32_t width, uint32_t height);

  private:
    uint32_t viewportWidth, viewportHeight;
    entt::registry eRegistry;

    friend class Entity;
};

} // namespace Chimera

#endif