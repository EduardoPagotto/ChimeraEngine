#pragma once
#include "../Registry.hpp"
#include "Collada.hpp"

namespace Chimera {
class ColladaGeometry : public Collada {
  public:
    ColladaGeometry(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaGeometry();
    void create(Entity& entity, pugi::xml_node geo);

  private:
};

} // namespace Chimera