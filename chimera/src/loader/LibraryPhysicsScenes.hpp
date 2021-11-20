#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/render/scene/Entity.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class LibraryPhysicsScenes : public Library {
  public:
    LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string& _url, Scene* scene) : Library(_root, _url), scene(scene) {}
    virtual ~LibraryPhysicsScenes() {}
    void target();

  private:
    void loadPhysicControlCollada(tinyxml2::XMLElement* _nNode);
    Scene* scene;
    Entity entity;
};
} // namespace Chimera
