#pragma once
#include "Collada.hpp"
#include "chimera/core/Registry.hpp"

namespace Chimera {
class ColladaCam : public Collada {
  public:
    ColladaCam(ColladaDom& dom, const std::string& url, std::shared_ptr<ServiceLocator> serviceLoc) : Collada(dom, url, serviceLoc) {};
    virtual ~ColladaCam() {}
    void create(Entity& entity, pugi::xml_node nodeCam);
    void createExtra(Entity& entity, pugi::xml_node node);
};

} // namespace Chimera