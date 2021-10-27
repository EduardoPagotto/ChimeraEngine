#ifndef __CHIMERA_SCENE_ZZ1__HPP
#define __CHIMERA_SCENE_ZZ1__HPP

#include "chimera/core/ICamera.hpp"
#include "chimera/core/buffers/RenderBuffer.hpp"
#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/Light.hpp"
#include <entt/entt.hpp>

namespace Chimera {

class Entity;
class Scene {
  public:
    Scene();
    virtual ~Scene();
    Entity createEntity(const std::string& name = std::string());
    void destroyEntity(Entity entity);
    void addLight(Light* light) { lightSetupStack.push_back(light); }
    void setCamera(ICamera* camera) { this->camera = camera; }
    void onViewportResize(uint32_t width, uint32_t height);
    void render(IRenderer3d& renderer);
    void onUpdate(float ts);
    void onCreate();
    void onDestroy();

  protected:
    uint32_t viewportWidth, viewportHeight;
    entt::registry eRegistry;

    friend class Entity;

  private:
    // Shader shader;
    std::vector<Light*> lightSetupStack;
    ICamera* camera;
    RenderBuffer* renderBuffer;
};

} // namespace Chimera

#endif