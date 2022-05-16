#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaCube : public Collada {
  public:
    ColladaCube(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaCube();
    void create(const std::string& id, const std::string& name, Entity& entity, pugi::xml_node geo);

  private:
};

} // namespace Chimera