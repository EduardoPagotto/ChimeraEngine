#pragma once
#include "../Registry.hpp"
#include "Collada.hpp"

namespace Chimera {
class ColladaMaterial : public Collada {
  public:
    ColladaMaterial(ColladaDom& dom, const std::string& url) : Collada(dom, url){};
    virtual ~ColladaMaterial();
    void create(Entity& entity, const pugi::xml_node& node);
};
} // namespace Chimera
