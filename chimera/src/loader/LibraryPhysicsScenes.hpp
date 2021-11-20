#pragma once
#include "chimera/core/Registry.hpp"
#include "chimera/loader/Library.hpp"

namespace Chimera {

class LibraryPhysicsScenes : public Library {
  public:
    LibraryPhysicsScenes(tinyxml2::XMLElement* _root, const std::string& _url, Registry* reg) : Library(_root, _url), reg(reg) {}
    virtual ~LibraryPhysicsScenes() {}
    void target();

  private:
    void loadPhysicControlCollada(tinyxml2::XMLElement* _nNode);
    Registry* reg;
    Entity entity;
};
} // namespace Chimera
