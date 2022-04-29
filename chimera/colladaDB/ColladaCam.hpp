#pragma once
#include "chimera/colladaDB/Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaCam : public Collada {
  public:
    ColladaCam(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaCam();
    void create(Entity& entity, pugi::xml_node nodeCam);

  private:
};

} // namespace Chimera