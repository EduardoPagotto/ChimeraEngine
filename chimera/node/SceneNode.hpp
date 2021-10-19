#ifndef __CHIMERA_SCENE_NODE__HPP
#define __CHIMERA_SCENE_NODE__HPP

#include "Node.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class SceneNode : public Scene {
  public:
    SceneNode();
    virtual ~SceneNode();

    Node* getRoot() { return &this->root; }

    // Entity createEntity(const std::string& name = std::string());
    // void destroyEntity(Entity entity);
    // void onUpdate(float ts);

    // void onCreate();
    // void onDestroy();

    // void onViewportResize(uint32_t width, uint32_t height);

    //   protected:
    //     uint32_t viewportWidth, viewportHeight;
    //     entt::registry eRegistry;

    //     friend class Entity;
  private:
    Node root;
};

} // namespace Chimera

#endif