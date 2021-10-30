#ifndef __CHIMERA_SCENE_ZZ1__HPP
#define __CHIMERA_SCENE_ZZ1__HPP

#include "chimera/core/ICamera.hpp"
#include "chimera/core/buffers/RenderBuffer.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include <entt/entt.hpp>

namespace Chimera {

class Entity;
class Scene {
    friend class Entity;

  public:
    Scene();
    virtual ~Scene();
    Entity createEntity(const std::string& name = std::string());
    void destroyEntity(Entity entity);
    void render(IRenderer3d& renderer);
    void onUpdate(float ts);
    void onCreate();
    void onDestroy();

    void onViewportResize(uint32_t width, uint32_t height);

  private:
    uint32_t viewportWidth, viewportHeight;
    entt::registry eRegistry;
    // Shader shader;
    ICamera* camera;
    RenderBuffer* renderBuffer;
};

} // namespace Chimera

#endif