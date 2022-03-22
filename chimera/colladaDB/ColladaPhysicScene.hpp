#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaPhysicScene : public Collada {
  public:
    ColladaPhysicScene() : Collada(){};
    virtual ~ColladaPhysicScene();

    void loadNode(pugi::xml_node node, Registry* reg, const std::string& body, const std::string& target);
    void loadAll(pugi::xml_node node, Registry* reg);

  private:
};

} // namespace Chimera