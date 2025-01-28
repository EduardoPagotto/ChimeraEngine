#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaMaterial : public Collada {
  public:
    ColladaMaterial(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaMaterial() {}
    void create(Entity& entity, const pugi::xml_node& node);
};
} // namespace Chimera
