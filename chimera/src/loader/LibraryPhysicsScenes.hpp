#pragma once
#include "chimera/loader/Library.hpp"
#include "chimera/loader/ListNodes.hpp"
#include "chimera/render/scene/Entity.hpp"
#include "chimera/render/scene/Scene.hpp"

namespace Chimera {

class LibraryPhysicsScenes : public Library {
  public:
    LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string& _url, Scene* scene);
    virtual ~LibraryPhysicsScenes();
    void target();

  private:
    void loadPhysicControlCollada(tinyxml2::XMLElement* _nNode);

    ListNodes* pListNodes;
    Scene* scene;
    Entity entity;
};
} // namespace Chimera
