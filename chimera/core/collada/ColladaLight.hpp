#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/Components.hpp"

namespace Chimera {
class ColladaLight : public Collada {
  public:
    ColladaLight(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaLight();
    void create(Entity& entity, pugi::xml_node nodeLight);

  private:
};

} // namespace Chimera