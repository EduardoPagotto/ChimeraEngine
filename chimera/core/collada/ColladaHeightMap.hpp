#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaHeightMap : public Collada {
  public:
    ColladaHeightMap(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaHeightMap();
    void create(Entity& entity, pugi::xml_node geo);

  private:
};

} // namespace Chimera