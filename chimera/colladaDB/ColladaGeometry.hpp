#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/Components.hpp"

namespace Chimera {
class ColladaGeometry : public Collada {
  public:
    ColladaGeometry(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaGeometry();
    void create(Entity& entity, pugi::xml_node geo);

  private:
};

} // namespace Chimera