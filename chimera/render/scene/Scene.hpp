#ifndef __CHIMERA_SCENE_ZZ1__HPP
#define __CHIMERA_SCENE_ZZ1__HPP

#include "chimera/core/Camera.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include <entt/entt.hpp>

namespace Chimera {

class Entity;
class Scene {
  public:
    Scene();
    virtual ~Scene();

    void addEntity(Entity entity);
    void setCamera(Camera* camera) { this->camera = camera; }

    Entity createEntity(const std::string& name = std::string());
    void destroyEntity(Entity entity);
    void onUpdate(float ts);

    void onCreate();
    void onDestroy();

    void onViewportResize(uint32_t width, uint32_t height);

    void render(IRenderer3d& renderer);

  protected:
    uint32_t viewportWidth, viewportHeight;
    entt::registry eRegistry;

    friend class Entity;

  private:
    std::vector<Entity> entities;
    Camera* camera;
};

} // namespace Chimera

#endif