#pragma once

#include "chimera/render/3d/IRenderer3d.hpp"
#include "chimera/render/ICamera.hpp"
#include "chimera/render/buffer/RenderBuffer.hpp"
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
    void onUpdate(const uint32_t& count);
    void onCreate();
    void onDestroy();

    void onViewportResize(uint32_t width, uint32_t height);

  private:
    uint32_t viewportWidth, viewportHeight;
    entt::registry eRegistry;
    // Shader shader;
    ICamera* camera;
    RenderBuffer* renderBuffer;
    FrameBuffer* fb;
};
} // namespace Chimera
