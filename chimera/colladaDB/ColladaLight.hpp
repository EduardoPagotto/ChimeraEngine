#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/Components.hpp"

namespace Chimera {
class ColladaLight : public Collada {
  public:
    ColladaLight() : Collada(){};
    virtual ~ColladaLight();
    void create(Entity& entity, pugi::xml_node nodeParent);

  private:
};

} // namespace Chimera