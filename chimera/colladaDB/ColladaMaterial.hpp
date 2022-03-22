#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/Material.hpp"

namespace Chimera {
class ColladaMaterial : public Collada {
  public:
    ColladaMaterial() : Collada(){};
    virtual ~ColladaMaterial();
    Material& create(Entity& entity, pugi::xml_node nodeParent);

  private:
    Texture* loadImage(pugi::xml_node nodeParent, const std::string& url);
};
} // namespace Chimera
